#ifndef RNDGEN_H_INCLUDED
#define RNDGEN_H_INCLUDED



#include<map>
#include<iostream>
#include <cstdlib>
#include <ctime>
#include<string>
#include <sstream>
#include<cmath>


class TRandomNumberGenerator {
public:
  virtual ~TRandomNumberGenerator() {};
  virtual double Generate() const = 0;
};

class TRandomGeneratorCreator {
public:
  virtual TRandomNumberGenerator *CreateObject(std::map<std::string, std::string> &params) const = 0;
};

class UniformRandomGenerator: public TRandomNumberGenerator {
private:
  double a_, b_;
public:
  ~UniformRandomGenerator() {};
  UniformRandomGenerator(const double a, const double b):
      a_(std::min(a,b)), b_(std::max(a,b)){};
  double Generate() const{
    return a_ + (b_ - a_) * (double(std::rand()) / RAND_MAX) ;
  }
};

class TUniformGeneratorCreator: public TRandomGeneratorCreator {
public:
  TRandomNumberGenerator *CreateObject(std::map<std::string, std::string> &params) const{
    std::string a = params["a"];
    std::string b = params["b"];
    return new UniformRandomGenerator(std::strtod(a.c_str(), nullptr), std::strtod(b.c_str(), nullptr));
  }
};

class BernoulliRandomGenerator: public TRandomNumberGenerator {
private:
  double p_;
public:
  ~BernoulliRandomGenerator() {};
  BernoulliRandomGenerator(const double p):
      p_(p) {};
  double Generate() const{
    return std::rand() < (p_ * (double(RAND_MAX)+1e-11)) ;
  }
};

class TBernoulliGeneratorCreator: public TRandomGeneratorCreator {
public:
  TRandomNumberGenerator *CreateObject(std::map<std::string, std::string> &params) const{
    std::string p = params["p"];
    return new BernoulliRandomGenerator(std::strtod(p.c_str(), nullptr));
  }
};

double getStandartDestribution() {
  static UniformRandomGenerator unifRand(-1, 1);
  double r = 0;
  double u, v;
  while (r == 0 || r >= 1)  {
    u = unifRand.Generate();
    v = unifRand.Generate();
    r = u * u + v * v;
  }
  return u * sqrt(-2 * log(r) / r);
}

class GaussRandomGenerator: public TRandomNumberGenerator {
private:
  double m_, s_;
public:
  ~GaussRandomGenerator() {};
  GaussRandomGenerator(const double m, const double s):
      m_(m), s_(s) {};
  double Generate() const{
    return m_ + s_ * getStandartDestribution();
  }
};

class TGaussGeneratorCreator: public TRandomGeneratorCreator {
public:
  TRandomNumberGenerator *CreateObject(std::map<std::string, std::string> &params) const{
    std::string m = params["m"], s = params["s"];
    return new GaussRandomGenerator(std::strtod(m.c_str(), nullptr), std::strtod(s.c_str(), nullptr));
  }
};

class LogisticRandomGenerator: public TRandomNumberGenerator {
private:
  double m_, s_;
public:
  ~LogisticRandomGenerator() {};
  LogisticRandomGenerator(const double m, const double s):
      m_(m), s_(s) {};
  double Generate() const{
    static UniformRandomGenerator uniformDistribution(0, 1);
    double p = 0;
    while (p == 0 || p ==1){
        p = uniformDistribution.Generate();
    }
    return m_ + s_ * log(p / (1 - p));
  }

};

class TLogisticGeneratorCreator: public TRandomGeneratorCreator {
public:
  TRandomNumberGenerator *CreateObject(std::map<std::string, std::string> &params) const{
    std::string m = params["m"], s = params["s"];
    return new LogisticRandomGenerator(std::strtod(m.c_str(), nullptr), std::strtod(s.c_str(), nullptr));
  }
};

class TRandomFactory {
private:
  std::map<std::string, TRandomGeneratorCreator*> creators_;
  TRandomFactory() {};
  ~TRandomFactory() {};
  TRandomFactory(const TRandomFactory &) {};
  TRandomFactory &operator = (const TRandomFactory &) {return *this;};
public:
  static TRandomFactory &Instance() {
    static TRandomFactory Factory;
    static bool IsInitialized = false;
    static TLogisticGeneratorCreator logisticGeneratorCreator;
    static TGaussGeneratorCreator gaussGeneratorCreator;
    static TBernoulliGeneratorCreator bernoulliGeneratorCreator;
    static TUniformGeneratorCreator uniformGeneratorCreator;
    if (!IsInitialized) {
      IsInitialized = true;
      srand(time(NULL));
      Factory.RegistCreator("logistic", &logisticGeneratorCreator);
      Factory.RegistCreator("gauss", &gaussGeneratorCreator);
      Factory.RegistCreator("bernoulli", &bernoulliGeneratorCreator);
      Factory.RegistCreator("uniform", &uniformGeneratorCreator);
    }
    return Factory;
  };

  void RegistCreator(const std::string &type, TRandomGeneratorCreator* obj) {
    creators_[type] = obj;
  }

  TRandomNumberGenerator *MakeGenerator(const std::map<std::string, std::string> &params) {
    auto paramsCopy = params;
    auto it = creators_.find(paramsCopy["type"]);
    if (it == creators_.end()) {
      return NULL;
    }
    return (it -> second -> CreateObject(paramsCopy));
  };


};
#endif // RNDGEN_H_INCLUDED
