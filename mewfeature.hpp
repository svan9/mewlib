/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_FEATURE_LIB_SO2U
#define MEW_FEATURE_LIB_SO2U
#include <initializer_list>
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewlist.hpp"
#include "mewstack.hpp"
#include "mewiterator.hpp"

namespace mew {
  class Feature;
  typedef void* FeatureData;
  typedef Feature*(*FeatureFn)(FeatureData result);
  enum struct FeatureResultState: byte {
    FINISHED, PENDING, SPEEPING, ERRORED
  };
  
  typedef struct {
    FeatureResultState state;
    FeatureData data;
  } FeatureResult;
  
  class Feature {
  public: 
    virtual FeatureResult poll() = 0;
    virtual void destroy() = 0;
  };

  #define FeaturePending() (FeatureResult{ .state = FeatureResultState::PENDING})
  #define FeatureFinished(_val) (FeatureResult{ .state = FeatureResultState::FINISHED, .data = _val})
  #define FeatureSleeping(_val) (FeatureResult{ .state = FeatureResultState::FINISHED, .data = _val})
  
  namespace Features {

  class Done: public Feature {
  public:
    ////////////////////////////////////////////////////////////
    FeatureFn worker = nullptr;
    Feature*  left   = nullptr;
    Feature*  right  = nullptr;

    ////////////////////////////////////////////////////////////
    Done() {}
    
    ////////////////////////////////////////////////////////////
    Done(FeatureFn fn): worker(fn) {}
    
    ////////////////////////////////////////////////////////////
    FeatureResult poll() {
      if (left != nullptr) {
        FeatureResult result = left->poll();
        if (result.state == FeatureResultState::FINISHED) {
          this->right = this->worker(result.data);
          this->left->destroy();
          this->left = nullptr;
          return FeaturePending();
        }
        return result;
      } else {
        MewAssert(right != nullptr);
        return right->poll();
      }
    }

    ////////////////////////////////////////////////////////////
    void destroy() {
      if (left) left->destroy();
      if (right) right->destroy();
      delete this;      
    }
  };

  class Then: public Feature {
  public:
    ////////////////////////////////////////////////////////////
    FeatureData result;

    ////////////////////////////////////////////////////////////
    Then() {}
  
    ////////////////////////////////////////////////////////////
    FeatureResult poll() {
      return FeatureFinished(result);
    }

    ////////////////////////////////////////////////////////////
    void destroy() {
      delete this;      
    }
  };

  }

  class FeaturePoll {
  public:
    ////////////////////////////////////////////////////////////
    mew::stack<Feature, 8U> row;

    ////////////////////////////////////////////////////////////
    FeaturePoll() {}

    ////////////////////////////////////////////////////////////
    FeaturePoll& append(Feature& f) {
      row.push(f);
      return *this;
    }

    void poll() {
      for (int i = 0; i < row.size(); ++i) {
        row.at(i).poll();
      }
    }
  };

  class Promise {
  public:
    ////////////////////////////////////////////////////////////
    typedef FeatureResult(*generator_t)();
    generator_t gen;

    ////////////////////////////////////////////////////////////
    Promise(generator_t gen): gen(gen) { }

    void operator()() {

    }
  };
};

#endif