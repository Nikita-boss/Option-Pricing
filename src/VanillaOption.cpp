// Implementation of header file VanillaOption.hpp

#include "VanillaOption.hpp"

namespace PricingLibrary {

	/// @brief Default constructor
	/// @param payoff Payoff object
	VanillaOption::VanillaOption(const std::shared_ptr<Payoff>& payoff) :
					Option{payoff} {}

	/// @brief Construct with Payoff object and pricing engine
	/// @param payoff payoff object
	/// @param engine pricing engine object
	VanillaOption::VanillaOption(const std::shared_ptr<Payoff>& payoff,
								 const std::shared_ptr<PricingEngine>& engine) :
								Option{payoff}, _engine{engine} {}

	/// @brief Copy constructor
	/// @param source VanillaOption object
	VanillaOption::VanillaOption(const VanillaOption& source) : 
								 Option{source._payoff} 
	{
		if (source._engine != nullptr)
		{
			_engine = source._engine;
		}
	}

	/// @brief Copy assignemnt
	/// @param source VanillaOption object
	/// @return VanillaOption object
	VanillaOption& VanillaOption::operator= (const VanillaOption& source)
	{
		// self-assignment check
		if (this == &source)
		{
			return *this;
		}
		
		_payoff = source._payoff;
		_engine = source._engine;

		return *this;
	}

	/// @brief Default desctructor
	VanillaOption::~VanillaOption() {}

	/// @brief set pricing engine
	/// @param engine Pricing engine object
	void VanillaOption::setEngine(const std::shared_ptr<PricingEngine>& engine)
	{
		_engine = engine;
	}

	/// @brief option price
	/// @return price
	double VanillaOption::getPrice() const
	{
		return _engine->getEnginePrice(_payoff);
	}

	/// @brief option delta greek
	/// @return delta
	double VanillaOption::getDelta() const
	{
		return _engine->getEngineDelta(_payoff);
	}

	/// @brief option gamma greek
	/// @return gamma
	double VanillaOption::getGamma() const
	{
		return _engine->getEngineGamma(_payoff);
	}

	/// @brief option vega greek
	/// @return vega
	double VanillaOption::getVega() const
	{
		return _engine->getEngineVega(_payoff);
	}

	/// @brief option theta greek
	/// @return theta
	double VanillaOption::getTheta() const
	{
		return _engine->getEngineTheta(_payoff);
	}
}