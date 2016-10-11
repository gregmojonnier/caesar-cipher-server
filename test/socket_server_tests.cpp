#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/socket_server.h"
#include "socket_functions_helper_mock.h"
#include <string>

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::StrEq;

class SocketServerTests : public ::testing::Test {
    protected:
        virtual void SetUp()
        {
            ::testing::DefaultValue<int>::Set(1);
        }

        virtual void TearDown()
        {
            ::testing::DefaultValue<int>::Clear();
        }

        void EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT() {
            // one client connects, then next client connection is bad(-1) exiting server
            EXPECT_CALL(_mock_socket_functions, CreateListenerSocket(_));
            EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_))
                .WillOnce(Return(1))
                .WillOnce(Return(-1));
        }

    SocketServer _test_subject;
    MockSocketFunctionsHelper _mock_socket_functions;
};

TEST_F(SocketServerTests, StartServer_CreatingListeningSocketFails_ReturnsFalse)
{
    EXPECT_CALL(_mock_socket_functions, CreateListenerSocket(_))
        .WillOnce(Return(-1));
    
    bool started = _test_subject.StartServer(55555, _mock_socket_functions);

    EXPECT_FALSE(started);
}

TEST_F(SocketServerTests, StartServer_WaitingForClientConnectionFails_ReturnsFalse)
{
    EXPECT_CALL(_mock_socket_functions, CreateListenerSocket(_))
        .WillOnce(Return(1));
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_))
        .WillOnce(Return(-1));
    
    bool started = _test_subject.StartServer(55555, _mock_socket_functions);

    EXPECT_FALSE(started);
}

TEST_F(SocketServerTests, StartServer_WaitingForClientConnectionParameters_ListeningSocketFdIsUsedToWaitForClientConnection)
{
    int listening_socket_fd = 36;
    EXPECT_CALL(_mock_socket_functions, CreateListenerSocket(_))
        .WillOnce(Return(listening_socket_fd));
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(listening_socket_fd))
        .WillOnce(Return(-1));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}


TEST_F(SocketServerTests, StartServer_ClientConnects_ClientFdIsUsedToWaitForData)
{
    int client_socket_fd = 98;
    EXPECT_CALL(_mock_socket_functions, CreateListenerSocket(_));
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_))
        .WillOnce(Return(client_socket_fd))
        .WillRepeatedly(Return(-1));
    EXPECT_CALL(_mock_socket_functions, WaitForData(client_socket_fd, _, _))
        .WillOnce(Return(0));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_ClientClosesConnection_WaitsForNextClient)
{
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(Return(0));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}


TEST_F(SocketServerTests, StartServer_ClientSendsDataWithoutSpace_KeepsWaitingForMoreDataUntilSpaceIsSeen)
{
    char data_without_space[] = "abcdef";
    size_t data_len = strlen(data_without_space);
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .Times(2)
        .WillOnce(
                DoAll(
                    SetIncomingClientData(&data_without_space),
                    Return(data_len)
                )
        )
        .WillRepeatedly(Return(-1));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_ClientDatasFirstWordIsNotANumber_ServerClosesConnection)
{
    char data_with_non_number_first_word[] = "not_a_number message";
    size_t data_len = strlen(data_with_non_number_first_word);
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(
                DoAll(
                    SetIncomingClientData(&data_with_non_number_first_word),
                    Return(data_len)
                )
        );
    EXPECT_CALL(_mock_socket_functions, CloseConnection(_));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_ClientSendsACompleteAndValidMessage_ServerResponds)
{
    char valid_message[] = "3 a_message_needing_shift_of_three ";
    size_t data_len = strlen(valid_message);
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(
                DoAll(
                    SetIncomingClientData(&valid_message),
                    Return(data_len)
                )
        );
    EXPECT_CALL(_mock_socket_functions, SendData(_, _, _));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_ClientRequestsAShiftOfZero_ServerRespondsWithTheUnchangedMessage)
{
    char message_with_shift_of_zero[] = "0 a_message_needing_shift_of_zero ";
    size_t data_len = strlen(message_with_shift_of_zero);
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(
                DoAll(
                    SetIncomingClientData(&message_with_shift_of_zero),
                    Return(data_len)
                )
        );
    EXPECT_CALL(_mock_socket_functions, SendData(_, StrEq("a_message_needing_shift_of_zero "), _));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_ClientRequestsAShiftOfOne_ServerRespondsWithMessageShiftedOne)
{
    char message_with_shift_of_zero[] = "1 abc ";
    size_t data_len = strlen(message_with_shift_of_zero);
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(
                DoAll(
                    SetIncomingClientData(&message_with_shift_of_zero),
                    Return(data_len)
                )
        );
    EXPECT_CALL(_mock_socket_functions, SendData(_, StrEq("bcd "), _));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_ClientRequestsAShiftOfNegativeOne_ServerRespondsWithMessageShiftedNegativeOne)
{
    char message_with_shift_of_zero[] = "-1 bcd ";
    size_t data_len = strlen(message_with_shift_of_zero);
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(
                DoAll(
                    SetIncomingClientData(&message_with_shift_of_zero),
                    Return(data_len)
                )
        );
    EXPECT_CALL(_mock_socket_functions, SendData(_, StrEq("abc "), _));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_ClientSendsMessageWithMultipleRequests_ServerRespondsToAllRequestsCorrectly)
{
    char message_with_shift_of_zero[] = "1 abc -1 bcd ";
    size_t data_len = strlen(message_with_shift_of_zero);
    EXPECT_ONE_ClIENT_CONNECTION_THEN_EXIT();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(
                DoAll(
                    SetIncomingClientData(&message_with_shift_of_zero),
                    Return(data_len)
                )
        );
    EXPECT_CALL(_mock_socket_functions, SendData(_, StrEq("bcd abc "), _));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}
