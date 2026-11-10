#include <memory>
#include <string>

#include <gtest/gtest.h>
#include <prompt_capabilities/prompt_pose_runner.hpp>

class TestPromptPoseRunner : public capabilities2_runner::PromptPoseRunner
{
public:
  explicit TestPromptPoseRunner(const rclcpp::Node::SharedPtr & node)
  {
    node_ = node;
  }

  using PromptPoseRunner::generate_request;
  using PromptPoseRunner::param_on_success;
  using PromptPoseRunner::process_response;
  using PromptPoseRunner::response_is_success;
};

class PromptPoseRunnerTest : public ::testing::Test
{
protected:
  static void SetUpTestSuite() { rclcpp::init(0, nullptr); }
  static void TearDownTestSuite() { rclcpp::shutdown(); }
};

TEST_F(PromptPoseRunnerTest, BuffersPoseWithExplicitUuidAndForwardsIt)
{
  auto node = std::make_shared<rclcpp::Node>("test_pose_cache");
  TestPromptPoseRunner runner(node);
  capabilities2_events::EventParameters parameters;
  parameters.set_value("uuid", std::string("prompt-test-uuid"), capabilities2_events::OptionType::STRING);
  parameters.set_value("flush", false, capabilities2_events::OptionType::BOOL);
  parameters.set_value("x", 1.0, capabilities2_events::OptionType::DOUBLE);

  const auto request = runner.generate_request(parameters);
  EXPECT_EQ(request.uuid, "prompt-test-uuid");
  EXPECT_TRUE(request.prompt.use_cache);
  EXPECT_FALSE(request.prompt.flush_cache);
  EXPECT_NE(request.prompt.prompt.find("x: 1.000000"), std::string::npos);

  auto response = std::make_shared<prompt_msgs::srv::Prompt::Response>();
  response->uuid = request.uuid;
  response->response.buffered = true;
  response->response.success = false;
  runner.process_response(response);
  EXPECT_TRUE(runner.response_is_success());
  auto output = runner.param_on_success();
  EXPECT_EQ(std::any_cast<std::string>(output.get_value("uuid", std::string{})), request.uuid);
}

TEST_F(PromptPoseRunnerTest, SupportsMissingUuidAndFlush)
{
  auto node = std::make_shared<rclcpp::Node>("test_pose_cache_defaults");
  TestPromptPoseRunner runner(node);
  capabilities2_events::EventParameters parameters;
  const auto request = runner.generate_request(parameters);
  EXPECT_TRUE(request.uuid.empty());
  EXPECT_TRUE(request.prompt.use_cache);
  EXPECT_FALSE(request.prompt.flush_cache);

  parameters.set_value("uuid", std::string("cache-id"), capabilities2_events::OptionType::STRING);
  parameters.set_value("flush", true, capabilities2_events::OptionType::BOOL);
  const auto flush_request = runner.generate_request(parameters);
  EXPECT_EQ(flush_request.uuid, "cache-id");
  EXPECT_TRUE(flush_request.prompt.use_cache);
  EXPECT_TRUE(flush_request.prompt.flush_cache);
}

TEST_F(PromptPoseRunnerTest, RejectsInvalidBufferedAndFailedResponses)
{
  auto node = std::make_shared<rclcpp::Node>("test_pose_cache_failure");
  TestPromptPoseRunner runner(node);
  auto response = std::make_shared<prompt_msgs::srv::Prompt::Response>();
  response->response.buffered = true;
  runner.process_response(response);
  EXPECT_FALSE(runner.response_is_success());

  response->response.buffered = false;
  response->response.success = false;
  runner.process_response(response);
  EXPECT_FALSE(runner.response_is_success());

  response->response.success = true;
  runner.process_response(response);
  EXPECT_TRUE(runner.response_is_success());
}