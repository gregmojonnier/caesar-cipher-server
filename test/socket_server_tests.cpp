#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/socket_server.h"
#include "socket_functions_helper_mock.h"
#include <string>

using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;

class SocketServerTests : public ::testing::Test {
    protected:
        virtual void SetUp()
        {
        }
        virtual void TearDown()
        {
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

TEST_F(SocketServerTests, StartServer_CreatingListeningSocketWorks_UsesListeningSocketFdToWaitForClients)
{
    EXPECT_CALL(_mock_socket_functions, CreateListenerSocket(_))
        .WillOnce(Return(36));
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(36))
        .WillOnce(Return(-1));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}

TEST_F(SocketServerTests, StartServer_WaitingForClientConnectionFails_ReturnsFalse)
{
    NiceMock<MockSocketFunctionsHelper> _mock_socket_functions;
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_))
        .WillOnce(Return(-1));
    
    bool started = _test_subject.StartServer(55555, _mock_socket_functions);

    EXPECT_FALSE(started);
}

TEST_F(SocketServerTests, StartServer_ReceivesClientConnection_UsesConnectionsSocketFdToWaitForData)
{
    NiceMock<MockSocketFunctionsHelper> _mock_socket_functions;
    EXPECT_CALL(_mock_socket_functions, WaitForClientConnection(_))
        .WillOnce(Return(98))
        .WillRepeatedly(Return(-1));
    EXPECT_CALL(_mock_socket_functions, WaitForData(98, _, _))
        .WillOnce(Return(0));
    
    _test_subject.StartServer(55555, _mock_socket_functions);
}
