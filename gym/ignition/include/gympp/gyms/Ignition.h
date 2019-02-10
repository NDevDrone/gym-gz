#ifndef GYMPP_GYMS_IGNITION
#define GYMPP_GYMS_IGNITION

#include "gympp/Gympp.h"

// TODO: fix forward decl
//#include <ignition/gazebo/Server.hh>

#include <memory>

namespace gympp {
    namespace gyms {
        template <typename AType, typename OType>
        class IgnitionGazebo;
    } // namespace gyms
} // namespace gympp

// TODO
namespace ignition {
    namespace gazebo {
        inline namespace v0 {
            class Server;
        }
    } // namespace gazebo
} // namespace ignition

template <typename ActionDataType, typename ObservationDataType>
class EnvironmentBehavior
{
public:
    template <typename Type>
    using Buffer = std::valarray<Type>;

    using Action = Buffer<ActionDataType>;
    using Observation = Buffer<ObservationDataType>;

    using Reward = float;

    virtual bool isDone() = 0;
    virtual bool setAction(const Action& action) = 0;
    virtual std::optional<Reward> computeReward() = 0;
    virtual std::optional<Observation> getObservation() = 0;
};

template <typename AType, typename OType>
class gympp::gyms::IgnitionGazebo
    : public gympp::Environment
    , EnvironmentBehavior<AType, OType>
{
private:
    class Impl;
    std::unique_ptr<Impl> pImpl = nullptr;

protected:
    //    std::unique_ptr<ignition::gazebo::v0::Server> m_server = nullptr;
    std::unique_ptr<ignition::gazebo::Server> m_server = nullptr;

public:
    using Environment = gympp::Environment;
    using State = typename Environment::State;
    using Action = typename Environment::Action;
    using Reward = typename Environment::Reward;
    using Observation = typename Environment::Observation;
    using RenderMode = typename Environment::RenderMode;
    using ActionSpace = typename Environment::ActionSpace;
    using ObservationSpace = typename Environment::ObservationSpace;
    using ActionSpacePtr = typename Environment::ActionSpacePtr;
    using ObservationSpacePtr = typename Environment::ObservationSpacePtr;

    using TypedEnvironmentBehavior = EnvironmentBehavior<AType, OType>;

    IgnitionGazebo() = delete;
    IgnitionGazebo(const ActionSpacePtr aSpace,
                   const ObservationSpacePtr oSpace,
                   const std::string& sdfFile,
                   double updateRate,
                   uint64_t iterations = 1);
    ~IgnitionGazebo() override;

    bool render(RenderMode mode) override;
    std::optional<Observation> reset() override;
    std::optional<State> step(const Action& action) override;

    // Public APIs
    void setVerbosity(int level = 4);
    bool loadSDF(std::string& sdfFile);

    Environment* env() override;
    // TODO: const correctnessference to `gympp::gyms::IgnitionGazebo<unsigned long,
    // float>::getEnvironment() const'

    //    virtual State getState(const Action& action) = 0;

    bool isDone() override = 0;
    bool setAction(const typename TypedEnvironmentBehavior::Action& action) override = 0;
    std::optional<typename TypedEnvironmentBehavior::Reward> computeReward() override = 0;
    std::optional<typename TypedEnvironmentBehavior::Observation> getObservation() override = 0;
    //    std::optional<std::valarray<OType>> getObservation() override = 0;
};

// Instantiate the template for canonical problems
extern template class gympp::gyms::IgnitionGazebo<size_t, double>;

#endif // GYMPP_GYMS_IGNITION