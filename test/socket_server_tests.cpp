#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/socket_server.h"
#include "socket_functions_helper_mock.h"
#include <string>

using ::testing::_;
using ::testing::Return;

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

        void EXPECT_DEFAULT_CLIENT_CONNECTION() {
            EXPECT_CALL(_mock_socket_functions, CreateListenerSocket(_));
            EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_));
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
    ::testing::InSequence seq;
    EXPECT_DEFAULT_CLIENT_CONNECTION();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(Return(0));
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_))
        .WillOnce(Return(-1));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}


/*
TEST_F(SocketServerTests, StartServer_ClientClosesConnection_WaitsForNextClient)
{
    ::testing::InSequence seq;
    EXPECT_DEFAULT_CLIENT_CONNECTION();
    EXPECT_CALL(_mock_socket_functions, WaitForData(_, _, _))
        .WillOnce(Return(0));
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_))
        .WillOnce(Return(-1));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}
*/
