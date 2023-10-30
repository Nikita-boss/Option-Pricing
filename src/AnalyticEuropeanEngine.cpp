// Implementation of the header file AnalyticEuropeanEngine.hpp

#include "AnalyticEuropeanEngine.hpp"

namespace PricingLibrary {

	/// @brief Default constructor
	/// @param S underlying price
	/// @param sigma volatility
	/// @param r risk-free rate
	/// @param b cost-of-carry parameter
	AnalyticEuropeanEngine::AnalyticEuropeanEngine(double S, double sigma, double r, double b)
	: _S{S}, _sigma{sigma}, _r{r}, _b{b} {}

	/// @brief Copy constructor
	/// @param source AnalyticAmericanPerpetualEngine object
	AnalyticEuropeanEngine::AnalyticEuropeanEngine(const AnalyticEuropeanEngine& source)
	: PricingEngine{source}, _S{source._S}, _sigma{source._sigma}, _r{source._r}, _b{source._b} 
	{}

	/// @brief Copy assignemnt
	/// @param source AnalyticEuropeanEngine object
	/// @return AnalyticEuropeanEngine object
	AnalyticEuropeanEngine& AnalyticEuropeanEngine::operator= (const AnalyticEuropeanEngine& source)
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
	AnalyticEuropeanEngine::~AnalyticEuropeanEngine() {}

	/// @brief Validate if engine was passed to a correct option
	/// @param exercise American or European
	void AnalyticEuropeanEngine::validate(const Payoff::Exercise& exercise) const
	{
		if (exercise != Payoff::European)
		{
		throw IncorrectEngineException("Only European Options have analytic solution.");
		}
	}

	/// @brief return option price
	/// @param payoff Payoff object
	/// @return calls either call or option price function
	double AnalyticEuropeanEngine::getEnginePrice(const std::shared_ptr<Payoff>& payoff) const
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
	double AnalyticEuropeanEngine::getEngineCallPrice(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};

		double d1 = ( std::log(_S / K) + (_b + _sigma * _sigma / 2) * T ) / (_sigma * std::sqrt(T));
		double d2 = d1 - _sigma * std::sqrt(T);
		boost::math::normal_distribution standard_normal(0.0, 1.0);
		double N_d1 = boost::math::cdf(standard_normal, d1);
		double N_d2 = boost::math::cdf(standard_normal, d2);
		double price = _S * std::exp( (_b - _r) * T ) * N_d1 - K * std::exp(-_r * T) * N_d2;

		return price;
	}

	/// @brief return put option price
	/// @param payoff Payoff object
	/// @return price
	double AnalyticEuropeanEngine::getEnginePutPrice(const std::shared_ptr<Payoff>& payoff) const
	{
		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};
		double callPrice = getEngineCallPrice(payoff);

		return callPrice - _S + K * std::exp(-_r * T);
	}

	/// @brief return delta greek
	/// @param payoff Payoff object
	/// @return delta
	double AnalyticEuropeanEngine::getEngineDelta(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};
		boost::math::normal_distribution standard_normal(0.0, 1.0);
		double d1 = ( std::log(_S / K) + (_b + _sigma * _sigma / 2) * T ) / (_sigma * std::sqrt(T));
		d1 = (payoff->getType() == Payoff::Type::Call) ? d1 : -d1;
		double N_d1 = boost::math::cdf(standard_normal, d1);
		double delta = std::exp( (_b - _r ) * T ) * N_d1;

		return (payoff->getType() == Payoff::Type::Call) ? delta : -delta;
	}

	/// @brief return gamma greek
	/// @param payoff Payoff object
	/// @return gamma
	double AnalyticEuropeanEngine::getEngineGamma(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};
		boost::math::normal_distribution standard_normal(0.0, 1.0);
		double d1 = ( std::log(_S / K) + (_b + _sigma * _sigma / 2) * T ) / (_sigma * std::sqrt(T));
		double n_d1 = boost::math::pdf(standard_normal, d1);
		double gamma = n_d1 * std::exp( (_b - _r) * T ) / (_S * _sigma * std::sqrt(T));

		return gamma;
	}

	/// @brief return Vega greek
	/// @param payoff Payoff object
	/// @return vega
	double AnalyticEuropeanEngine::getEngineVega(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};
		boost::math::normal_distribution standard_normal(0.0, 1.0);
		double d1 = ( std::log(_S / K) + (_b + _sigma * _sigma / 2) * T ) / (_sigma * std::sqrt(T));
		double n_d1 = boost::math::pdf(standard_normal, d1);
		double vega = _S * std::sqrt(T) * std::exp( (_b - _r) * T ) * n_d1 / 100; // divide by 100 to covert from percentage to raw

		return vega;
	}

	/// @brief return Theta greek
	/// @param payoff Payoff object
	/// @return theta
	double AnalyticEuropeanEngine::getEngineTheta(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};
		double d1 = ( std::log(_S / K) + (_b + _sigma * _sigma / 2) * T ) / (_sigma * std::sqrt(T));
		double d2 = d1 - _sigma * std::sqrt(T);
		boost::math::normal_distribution standard_normal(0.0, 1.0);
		double N_d1 = (payoff->getType() == Payoff::Type::Call) ? 
					boost::math::cdf(standard_normal, d1) :  boost::math::cdf(standard_normal, -d1);
		double N_d2 = (payoff->getType() == Payoff::Type::Call) ?
		 			boost::math::cdf(standard_normal, d2) : boost::math::cdf(standard_normal, -d2);
		double n_d1 = boost::math::pdf(standard_normal, d1);

		double theta{};
		if (payoff->getType() == Payoff::Type::Call)
		{
			theta = -(_S * _sigma * std::exp( (_b - _r) * T ) * n_d1) / (2 * std::sqrt(T)) 
				   - (_b - _r) * _S * std::exp( (_b - _r) * T ) * N_d1
				   - _r * K * std::exp(-_r * T) * N_d2;
		}
		else
		{
			theta = -(_S * _sigma * std::exp( (_b - _r) * T ) * n_d1) / (2 * std::sqrt(T)) 
				   + (_b - _r) * _S * std::exp( (_b - _r) * T ) * N_d1
				   + _r * K * std::exp(-_r * T) * N_d2;
		}
		 
		return theta;
	}

	/// @brief if put-call parity is satisfied
	/// @param call call option
	/// @param put put option
	/// @param K strike price
	/// @param S underlying price
	/// @param r risk-free rate
	/// @param T maturity
	/// @param epsilon double comparison error
	/// @return boolean
	bool AnalyticEuropeanEngine::satisfy_put_call_parity(double call, double put, double K, 
													     double S, double r, double T, 
														 double epsilon)
	{
		return std::abs(call - put - S + K * std::exp(-r * T)) <= epsilon;
	}
	
}