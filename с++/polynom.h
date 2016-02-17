#include<vector>
#include<iostream>

namespace NPolynom {
  template <class T>
  class TPolynom
  {
  private:
    std::vector<T> coefficents;

    void correct(){
      if (not coefficents.empty()){
        while (not coefficents.back() && not coefficents.empty()) {
          coefficents.pop_back();
        }
      }
    }


  public:
    TPolynom<T>():
      coefficents(0)
      {}

    TPolynom<T>(const T a_0):
      coefficents(1)
      {coefficents[0]=a_0;}

    TPolynom<T>(const std::vector<T> vector):
      coefficents(vector){
        correct();
      }

    template<typename T1>
    TPolynom<T> (const TPolynom<T1>& arg):
      coefficents() {
        if (arg.Degree()==-1){

          return;
        }
        coefficents.resize(arg.Degree() + 1, 0);
        for (int i = arg.Degree(); i >= 0; --i) {
          coefficents[i] = (T)(arg.get(i));}
        correct();
      }


    template<typename T1>
    TPolynom& operator = (const TPolynom<T1>& other) {
      int max=std::max(Degree(), other.Degree());
      if (max==-1) {
        return *this;
      }
      for (int i = max; i >= 0; --i) {
        (*this)[i] = (T)(other.get(i));}
      return *this;
    }

    bool operator == (TPolynom that) const{
      int deg1 = Degree();
      if (deg1 != that.Degree()) {
        return false;
      } else {
        for (int i = 0; i <= deg1; ++i) {
          if (coefficents[i] != that[i]) {
            return false;
          }
        }
      }
      return true;
    }

    const bool operator != (const TPolynom other) const {
      return !(operator==(other));
    }

    TPolynom<T> operator-() const{
      TPolynom<T> result(*this);
      for (auto it=result.begin(); it!=result.end(); ++it) {
        *it=-(*it);
      }
      return result;
    }

    TPolynom<T> operator+() const{
      TPolynom<T> result(*this);
      for (auto it=result.begin(); it!=result.end(); ++it) {
        *it=+(*it);
      }
      return result;
    }

    TPolynom<T> operator+=(const TPolynom& P) {
      if (P.Degree() >= coefficents.size())
        coefficents.resize(P.Degree() + 1, 0);
      for (int deg = 0; deg <= P.Degree(); ++deg) {
        coefficents[deg]+=P.coefficents[deg];
      }
      return *this;
    }

    template <class T1>
    TPolynom<T> operator+=(const T1 a) {
      operator[](0)+=a;
      return *this;
    }

    TPolynom<T> operator-=(const TPolynom& P) {
      if (P.Degree() >= coefficents.size())
        coefficents.resize(P.Degree() + 1, 0);
      for (int deg = 0; deg <= P.Degree(); ++deg) {
        coefficents[deg]+=P.coefficents[deg];
      }
      return *this;
    }

    template <class T1>
    TPolynom<T> operator-=(const T1 a) {
      operator[](0)-=a;
      return *this;
    }



    TPolynom<T> operator* (const TPolynom<T> Q) const {
        int deg1=Degree();
        int deg2 = Q.Degree();
        if ((Degree() == -1) || (deg2 == -1)) {
          TPolynom<T> result(0);
          return result;
        }
        TPolynom<T> result;
        int max_deg = Degree() + Q.Degree();
        result.coefficents.resize(max_deg + 1, 0);
        for (int deg = 0; deg <= max_deg; ++deg) {
          for (int i = std::max(deg-deg2, 0); ((i <= deg1) && ((deg - i) >= 0)); ++i) {
            result.coefficents[deg]+=(get(i)*Q.get(deg-i));
          }
        }
        return result;
    }

    TPolynom<T> operator*=(const TPolynom& P) {
      *this = P * (*this);
      return *this;
    }

    template <class numeric_type>
    TPolynom<T> operator*=(const numeric_type a) {
      for (auto it = begin(); it != end(); ++it){
        (*it) *= a;
      }
      return *this;
    }

    template <class numeric_type>
    TPolynom<T> operator/=(const numeric_type a) {
      if (Degree()==-1) {
        return *this;
      }
      for (auto it = begin(); it != end(); ++it){
        (*it) /= a;
      }
      return *this;
    }

    int Degree() const {
      int i = coefficents.size()-1;
      while ( i >= 0) {
        if (coefficents[i] != 0) {
          return i;
        }
        --i;
      }
      return(i);
    }

    T& operator[] (size_t n);
    T operator[] (size_t n) const;


    template<class numeric_type>
     T operator () (numeric_type x) const{
      T result=0;
      for (int i=Degree(); i>=0; --i) {
        result *= x;
        result += coefficents[i];
      }
      return result;
    }

    typedef typename __gnu_cxx::__normal_iterator<T*, std::vector<T>> iterator;
    iterator begin(){
      return (coefficents.begin());
    }
    iterator end(){
      return (coefficents.end());
    }

    T get(int deg) const {
      if (deg > Degree())
        return 0;
      return coefficents[deg];
    }

    template <class T2>
    TPolynom<T> operator/ ( const TPolynom<T2>& Q) {
      int deg2 = Q.Degree();
      if (deg2 == -1) {
        return 0;
      }
      TPolynom<T> R(*this);
      TPolynom<T> result(0);
      T2 q = Q.get(deg2);
      T temp = 0;
      while (R.Degree() >= deg2) {
        temp = R.get(R.Degree())/q;
        result[R.Degree() - deg2] = temp;
        R[R.Degree()] = 0;
        int R_deg=R.Degree();
        for (int i=0; i < deg2; ++i) {
          R[R_deg-i] -= (temp * Q.get(deg2 - i-1));
        }
        R.correct();
      }
      return result;
    }

    template <class T2>
    TPolynom<T> operator% ( const TPolynom<T2>& Q) {
      int deg2 = Q.Degree();
      if (deg2 == -1) {
        return *this;
      }
      TPolynom<T> R(*this);
      T2 q = Q.get(deg2);
      T temp = 0;
      while (R.Degree() >= deg2) {
        temp = R.get(R.Degree())/q;
        R[R.Degree()] = 0;
        int R_deg=R.Degree();
        for (int i=0; i < deg2; ++i) {
          R[R_deg-i] -= (temp * Q.get(deg2 - i-1));
        }
        R.correct();
      }
      return R;
    }

    template <class T2>
    TPolynom<T> operator/= ( const TPolynom<T2>& Q) {
      (*this) = operator/(Q);
      return *this;
    }

    template <class T2>
    TPolynom<T> operator%= ( const TPolynom<T2>& Q) {
      (*this) = operator%(Q);
      return *this;
    }

  TPolynom<T> operator, ( const TPolynom<T>& Q1) const {
      if (Q1.Degree() == -1) {
        return *this;
      }
      TPolynom<T> Q(Q1);
      TPolynom<T> P(*this);
      TPolynom<T> temp(0);
      while (Q.Degree() != -1) {
        temp = Q;
        Q = P % Q;
        P = temp;
      }

      if (*(P.begin()) < 0) {
        P = -P;
      }

      return P;
    }


    template <class T1>
    friend TPolynom<T1> operator+ (const TPolynom<T1>& P, const TPolynom<T1>& Q);
    template <class T1>
    friend TPolynom<T1> operator- (const TPolynom<T1>& P, const TPolynom<T1>& Q);
    template <class T1,  class numeric_type>
    friend TPolynom<T1> operator+ (const TPolynom<T1>& P, const numeric_type num);
    template <class T1,  class numeric_type>
    friend TPolynom<T1> operator- (const TPolynom<T1>& P, const numeric_type num);
    template <class T1,  class numeric_type>
    friend TPolynom<T1> operator* (const TPolynom<T1>& P, const numeric_type num);
    template <class T1, class numeric_type>
    friend TPolynom<T1> operator/ (const TPolynom<T1>& P, const numeric_type num);
  };


  template <class T>
  TPolynom<T> operator+ (const TPolynom<T>& P, const TPolynom<T>& Q) {
    TPolynom<T> result(P);
    result+=Q;
    result.correct();
    return result;
  }

  template <class T, class numeric_type>
  TPolynom<T> operator+ (const TPolynom<T>& P, const numeric_type num) {
    TPolynom<T> result(P);
    result+=num;
    result.correct();
    return result;
  }

  template <class T>
  TPolynom<T> operator- (const TPolynom<T>& P, const TPolynom<T>& Q) {
    TPolynom<T> result(P);
    result-=Q;
    result.correct();
    return result;
  }

  template <class T, class numeric_type>
  TPolynom<T> operator- (const TPolynom<T>& P, const numeric_type num) {
    TPolynom<T> result(P);
    result-=num;
    result.correct();
    return result;
  }

  template <class T, class numeric_type>
  TPolynom<T> operator* (const TPolynom<T>& P, const numeric_type num) {
    TPolynom<T> result(P);
    result*=num;
    result.correct();
    return result;
  }

  template <class T>
  std::ostream& operator << (std::ostream& stream, const TPolynom<T> P) {
    if (P.Degree() == -1) {
      stream << 0;
      return stream;
    }
    for (int deg = P.Degree(); deg>0; --deg) {
      if (P.get(deg) == 1) {
        stream << "x^" << deg;
      } else if (P.get(deg) == -1) {
        stream << "-x^" << deg;
      } else if (P.get(deg) != 0) {
        stream << P.get(deg) << 'x';
        if (deg != 1)
          stream << '^'<< deg;
      }
      if (P.get(deg-1) > 0)
          stream << '+';
    }
    if (P.get(0) != 0)
      stream << P.get(0);
    return stream;
  }

  template <class T, class numeric_type>
  TPolynom<T> operator/ (const TPolynom<T>& P, const numeric_type num) {
    TPolynom<T> result(P);
    result/=num;
    result.correct();
    return result;
  }

  template <typename T>
  T& TPolynom<T>::operator[] (size_t deg) {
       if (deg >= this->coefficents.size()) {
           this->coefficents.resize(deg + 1, 0);
       }
       return this->coefficents[deg];
  }

  template <typename T>
  T TPolynom<T>::operator[] (size_t deg) const {
     if (deg >= this->coefficents.size()) {
         return 0;
     }
     return this->coefficents[deg];
  }


}
