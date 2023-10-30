// Implementation of the header file AnalyticAmericanPerpetualEngine.hpp

#include "AnalyticAmericanPerpetualEngine.hpp"

namespace PricingLibrary {

	/// @brief Default constructor
	/// @param S underlying price
	/// @param sigma volatility
	/// @param r risk-free rate
	/// @param b cost-of-carry parameter
	AnalyticAmericanPerpetualEngine::AnalyticAmericanPerpetualEngine(double S, double sigma, double r, double b)
	: _S{S}, _sigma{sigma}, _r{r}, _b{b} {}

	/// @brief Copy constructor
	/// @param source AnalyticAmericanPerpetualEngine object
	AnalyticAmericanPerpetualEngine::AnalyticAmericanPerpetualEngine(const AnalyticAmericanPerpetualEngine& source)
	: PricingEngine{source}, _S{source._S}, _sigma{source._sigma}, _r{source._r}, _b{source._b} 
	{}

	/// @brief Copy assignemnt
	/// @param source AnalyticAmericanPerpetualEngine object
	/// @return AnalyticAmericanPerpetualEngine object
	AnalyticAmericanPerpetualEngine& AnalyticAmericanPerpetualEngine::operator= (const AnalyticAmericanPerpetualEngine& source)
	{
		// check for self-assignment
		if (this == &source)
		{
			return *this;
		}

		PricingEngine::operator=(source);
		_S = source._S;
		_sigma = source._sigma;
		_r = source._r;
		_b = source._b;

		return *this;
	}

	/// @brief Default destructor
	AnalyticAmericanPerpetualEngine::~AnalyticAmericanPerpetualEngine() {}

	/// @brief Validate if engine was passed to a correct option
	/// @param exercise American or European
	void AnalyticAmericanPerpetualEngine::validate(const Payoff::Exercise& exercise) const
	{
		if (exercise != Payoff::American)
		{
		throw IncorrectEngineException("Only American options have perpetual engine pricing.");
		}
	}

	/// @brief return option price
	/// @param payoff Payoff object
	/// @return calls either call or option price function
	double AnalyticAmericanPerpetualEngine::getEnginePrice(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		if (payoff->getType() == Payoff::Type::Call)
		{
			return getEngineCallPrice(payoff);
		}
		else
		{
			return getEnginePutPrice(payoff);
		}
	}

	/// @brief return call option price
	/// @param payoff Payoff object
	/// @return price
	double AnalyticAmericanPerpetualEngine::getEngineCallPrice(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double y1 = 0.5 - _b / (_sigma * _sigma) 
				  + std::sqrt( std::pow( _b / (_sigma * _sigma) - 0.5, 2) 
							   + (2 * _r) / (_sigma * _sigma) );
		double price = payoff->getStrike() / (y1 - 1) 
					 * std::pow( ((y1 - 1) * _S) / (y1 * payoff->getStrike()), y1 );

		return price;
	}

	/// @brief return put option price
	/// @param payoff Payoff object
	/// @return price
	double AnalyticAmericanPerpetualEngine::getEnginePutPrice(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double y2 = 0.5 - _b / (_sigma * _sigma) 
				  - std::sqrt( std::pow( _b / (_sigma * _sigma) - 0.5, 2) 
							   + (2 * _r) / (_sigma * _sigma) );
		double price = payoff->getStrike() / (1 - y2) 
					 * std::pow( ((y2 - 1) * _S) / (y2 * payoff->getStrike()), y2 );

		return price;
	}


	/*! \warning Not implemented calculation of Delta greek */
	double AnalyticAmericanPerpetualEngine::getEngineDelta(const std::shared_ptr<Payoff>& payoff) const
	{
		return -1.0;
	}

	/*! \warning Not implemented calculation of Gamma greek */
	double AnalyticAmericanPerpetualEngine::getEngineGamma(const std::shared_ptr<Payoff>& payoff) const
	{
		return -1.0;
	}
	
	/*! \warning Not implemented calculation of Theta greek */
	double AnalyticAmericanPerpetualEngine::getEngineTheta(const std::shared_ptr<Payoff>& payoff) const
	{
		return -1.0;
	}

	/*! \warning Not implemented calculation of Vega greek */
	double AnalyticAmericanPerpetualEngine::getEngineVega(const std::shared_ptr<Payoff>& payoff) const
	{
		return -1.0;
	}
}