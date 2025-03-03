/**
 * DeepDetect
 * Copyright (c) 2014-2016 Emmanuel Benazera
 * Author: Emmanuel Benazera <beniz@droidnik.fr>
 *
 * This file is part of deepdetect.
 *
 * deepdetect is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * deepdetect is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with deepdetect.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SERVICES_H
#define SERVICES_H

#ifdef USE_DD_SYSLOG
#define SPDLOG_ENABLE_SYSLOG
#endif

#include <mapbox/variant.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include "mlservice.h"
#include "apidata.h"
#include "inputconnectorstrategy.h"
#include "imginputfileconn.h"
#include "csvinputfileconn.h"
#include "txtinputfileconn.h"
#include "svminputfileconn.h"
#include "allconnectors.hpp"
#include "outputconnectorstrategy.h"
#include "chain.h"
#include "chain_actions.h"
#include "resources.h"
#include "dto/service_predict.hpp"
#include "dto/chain.hpp"
#include "dto/stream.hpp"
#include "dto/resource.hpp"
#ifdef USE_CAFFE
#include "backends/caffe/caffelib.h"
#endif
#ifdef USE_TF
#include "backends/tf/tflib.h"
#endif
#ifdef USE_DLIB
#include "backends/dlib/dliblib.h"
#endif
#ifdef USE_CAFFE2
#include "backends/caffe2/caffe2lib.h"
#endif
#ifdef USE_XGBOOST
#include "backends/xgb/xgblib.h"
#endif
#ifdef USE_TSNE
#include "backends/tsne/tsnelib.h"
#endif
#ifdef USE_NCNN
#include "backends/ncnn/ncnnlib.h"
#endif
#ifdef USE_TORCH
#include "backends/torch/torchlib.h"
#endif
#ifdef USE_TENSORRT
#include "backends/tensorrt/tensorrtlib.h"
#endif
#include "dd_spdlog.h"
#include <vector>
#include <mutex>
#include <chrono>
#include <iostream>

namespace dd
{
  /* service types as variant type. */
  typedef mapbox::util::variant<
#ifdef USE_CAFFE
      MLService<CaffeLib, ImgCaffeInputFileConn, SupervisedOutput, CaffeModel>,
      MLService<CaffeLib, CSVCaffeInputFileConn, SupervisedOutput, CaffeModel>,
      MLService<CaffeLib, CSVTSCaffeInputFileConn, SupervisedOutput,
                CaffeModel>,
      MLService<CaffeLib, TxtCaffeInputFileConn, SupervisedOutput, CaffeModel>,
      MLService<CaffeLib, SVMCaffeInputFileConn, SupervisedOutput, CaffeModel>,
      MLService<CaffeLib, ImgCaffeInputFileConn, UnsupervisedOutput,
                CaffeModel>,
      MLService<CaffeLib, CSVCaffeInputFileConn, UnsupervisedOutput,
                CaffeModel>,
      MLService<CaffeLib, CSVTSCaffeInputFileConn, UnsupervisedOutput,
                CaffeModel>,
      MLService<CaffeLib, TxtCaffeInputFileConn, UnsupervisedOutput,
                CaffeModel>,
      MLService<CaffeLib, SVMCaffeInputFileConn, UnsupervisedOutput,
                CaffeModel>
#endif
#ifdef USE_CAFFE2
#ifdef USE_CAFFE
      ,
#endif
      MLService<Caffe2Lib, ImgCaffe2InputFileConn, SupervisedOutput,
                Caffe2Model>,
      MLService<Caffe2Lib, ImgCaffe2InputFileConn, UnsupervisedOutput,
                Caffe2Model>
#endif
#ifdef USE_TF
#if defined(USE_CAFFE) || defined(USE_CAFFE2)
      ,
#endif
      MLService<TFLib, ImgTFInputFileConn, SupervisedOutput, TFModel>,
      MLService<TFLib, ImgTFInputFileConn, UnsupervisedOutput, TFModel>
#endif
#ifdef USE_DLIB
#if defined(USE_CAFFE) || defined(USE_CAFFE2) || defined(USE_TF)
      ,
#endif
      MLService<DlibLib, ImgDlibInputFileConn, SupervisedOutput, DlibModel>,
      MLService<DlibLib, ImgDlibInputFileConn, UnsupervisedOutput, DlibModel>
#endif
#ifdef USE_XGBOOST
#if defined(USE_CAFFE) || defined(USE_CAFFE2) || defined(USE_TF)              \
    || defined(USE_DLIB)
      ,
#endif
      MLService<XGBLib, CSVXGBInputFileConn, SupervisedOutput, XGBModel>,
      MLService<XGBLib, SVMXGBInputFileConn, SupervisedOutput, XGBModel>,
      MLService<XGBLib, TxtXGBInputFileConn, SupervisedOutput, XGBModel>
#endif
#ifdef USE_TSNE
#if defined(USE_CAFFE) || defined(USE_CAFFE2) || defined(USE_TF)              \
    || defined(USE_DLIB) || defined(USE_XGBOOST)
      ,
#endif
      MLService<TSNELib, CSVTSNEInputFileConn, UnsupervisedOutput, TSNEModel>,
      MLService<TSNELib, TxtTSNEInputFileConn, UnsupervisedOutput, TSNEModel>
#endif
#ifdef USE_NCNN
#if defined(USE_CAFFE) || defined(USE_CAFFE2) || defined(USE_TF)              \
    || defined(USE_DLIB) || defined(USE_XGBOOST) || defined(USE_TSNE)
      ,
#endif
      MLService<NCNNLib, CSVTSNCNNInputFileConn, SupervisedOutput, NCNNModel>,
      MLService<NCNNLib, ImgNCNNInputFileConn, SupervisedOutput, NCNNModel>
#endif
#ifdef USE_TORCH
#if defined(USE_CAFFE) || defined(USE_CAFFE2) || defined(USE_TF)              \
    || defined(USE_DLIB) || defined(USE_XGBOOST) || defined(USE_TSNE)         \
    || defined(USE_NCNN)
      ,
#endif
      MLService<TorchLib, ImgTorchInputFileConn, SupervisedOutput, TorchModel>,
      MLService<TorchLib, VideoTorchInputFileConn, SupervisedOutput,
                TorchModel>,
      MLService<TorchLib, TxtTorchInputFileConn, SupervisedOutput, TorchModel>,
      MLService<TorchLib, CSVTSTorchInputFileConn, SupervisedOutput,
                TorchModel>
#endif
#ifdef USE_TENSORRT
#if defined(USE_CAFFE) || defined(USE_CAFFE2) || defined(USE_TF)              \
    || defined(USE_DLIB) || defined(USE_XGBOOST) || defined(USE_TSNE)         \
    || defined(USE_NCNN) || defined(USE_TORCH)
      ,
#endif
      MLService<TensorRTLib, ImgTensorRTInputFileConn, SupervisedOutput,
                TensorRTModel>
#endif
      >
      mls_variant_type;

  class ServiceForbiddenException : public std::exception
  {
  public:
    ServiceForbiddenException(const std::string &s) : _s(s)
    {
    }
    ~ServiceForbiddenException()
    {
    }
    const char *what() const noexcept
    {
      return _s.c_str();
    }

  private:
    std::string _s;
  };

  class ServiceNotFoundException : public std::exception
  {
  public:
    ServiceNotFoundException(const std::string &s) : _s(s)
    {
    }
    ~ServiceNotFoundException()
    {
    }
    const char *what() const noexcept
    {
      return _s.c_str();
    }

  private:
    std::string _s;
  };

  namespace visitor_mllib
  {
    /**
     * \brief service mllib.predict_job() visitor class
     */
    class v_predict_job
    {
    public:
      const APIData &_in;
      bool _chain;

      template <typename T>
      oatpp::Object<DTO::PredictBody> operator()(T &mllib)
      {
        return mllib.predict_job(_in, _chain);
      }
    };
    template <typename T>
    static oatpp::Object<DTO::PredictBody>
    predict_job(T &mllib, const APIData &in, bool chain)
    {
      visitor_mllib::v_predict_job v{ in, chain };
      return mapbox::util::apply_visitor(v, mllib);
    }

    /**
     * \brief service mllib.train_job() visitor class
     */
    class v_train_job
    {
    public:
      const APIData &_in;
      APIData &_out;
      template <typename T> int operator()(T &mllib)
      {
        return mllib.train_job(_in, _out);
      }
    };

    template <typename T>
    static int train_job(T &mllib, const APIData &in, APIData &out)
    {
      visitor_mllib::v_train_job v{ in, out };
      return mapbox::util::apply_visitor(v, mllib);
    }

    /**
     * \brief service mllib.training_job_status() visitor class
     */
    class v_training_job_status
    {
    public:
      const APIData &_in;
      APIData &_out;
      template <typename T> int operator()(T &mllib)
      {
        return mllib.training_job_status(_in, _out);
      }
    };

    template <typename T>
    static int training_job_status(T &mllib, const APIData &in, APIData &out)
    {
      visitor_mllib::v_training_job_status v{ in, out };
      return mapbox::util::apply_visitor(v, mllib);
    }

    /**
     * \brief service mllib.train_init() visitor class
     */
    class v_init
    {
    public:
      const APIData &_in;
      template <typename T> void operator()(T &mllib)
      {
        mllib.init(_in);
      }
    };

    template <typename T> static void init(T &mllib, const APIData &in)
    {
      visitor_mllib::v_init v{ in };
      mapbox::util::apply_visitor(v, mllib);
    }

    /**
     * \brief service mllib.training_job_delete() visitor class
     */
    class v_training_job_delete
    {
    public:
      const APIData &_in;
      APIData &_out;
      template <typename T> int operator()(T &mllib)
      {
        return mllib.training_job_delete(_in, _out);
      }
    };

    template <typename T>
    static int training_job_delete(T &mllib, const APIData &in, APIData &out)
    {
      visitor_mllib::v_training_job_delete v{ in, out };
      return mapbox::util::apply_visitor(v, mllib);
    }

    /**
     * \brief service mllib.kill_job() visitor class
     */

    class v_clear
    {
    public:
      const APIData &_ad;
      template <typename T> void operator()(T &mllib)
      {
        mllib.kill_jobs();
        if (_ad.has("clear"))
          {
            std::string clear = _ad.get("clear").get<std::string>();
            if (clear == "full")
              mllib.clear_full();
#ifdef USE_SIMSEARCH
            else if (clear == "lib")
              {
                mllib.clear_mllib(_ad);
                mllib.clear_index();
              }
#else
            else if (clear == "lib")
              mllib.clear_mllib(_ad);
#endif
            else if (clear == "dir")
              mllib.clear_dir();
#ifdef USE_SIMSEARCH
            else if (clear == "index")
              mllib.clear_index();
#endif
          }
      }
    };

    template <typename T> static void clear(T &mllib, const APIData &ad)
    {
      visitor_mllib::v_clear v{ ad };
      return mapbox::util::apply_visitor(v, mllib);
    }

  };

  /**
   * \brief class for deepetect machine learning services.
   *        Each service instanciates a machine learning library and
   * channels data for training and prediction along with parameters from
   * API Service uses a variant type and store instances in a single
   * iterable container.
   */
  class Services
  {
  public:
    Services()
    {
    }
    ~Services()
    {
    }

    /**
     * \brief get number of services
     * @return number of service instances
     */
    size_t services_size() const
    {
      return _mlservices.size();
    }

    /**
     * \brief add a new service
     * @param sname service name
     * @param mls service object as variant
     * @param ad optional root data object holding service's parameters
     */
    void add_service(const std::string &sname, mls_variant_type &&mls,
                     const APIData &ad = APIData())
    {
      std::unordered_map<std::string, mls_variant_type>::const_iterator hit;
      if ((hit = _mlservices.find(sname)) != _mlservices.end())
        {
          throw ServiceForbiddenException("Service already exists");
        }

      auto llog = spdlog::get(sname);
      try
        {
          visitor_mllib::init(mls, ad);
          std::lock_guard<std::mutex> lock(_mlservices_mtx);
          _mlservices.insert(
              std::pair<std::string, mls_variant_type>(sname, std::move(mls)));
        }
      catch (InputConnectorBadParamException &e)
        {
          llog->error("service creation input connector bad param: {}",
                      e.what());
          throw;
        }
      catch (MLLibBadParamException &e)
        {
          llog->error("service creation mllib bad param: {}", e.what());
          throw;
        }
      catch (MLLibInternalException &e)
        {
          llog->error("service creation mllib internal error: {}", e.what());
          throw;
        }
      catch (...)
        {
          llog->error("service creation call failed: {}",
                      boost::current_exception_diagnostic_information());
          throw;
        }
    }

    /**
     * \brief removes and destroys a service
     * @param sname service name
     * @param ad root data object
     * @return true if service was removed, false otherwise (i.e. not
     * found)
     */
    bool remove_service(const std::string &sname, const APIData &ad)
    {
      std::lock_guard<std::mutex> lock(_mlservices_mtx);
      auto hit = _mlservices.begin();
      if ((hit = _mlservices.find(sname)) != _mlservices.end())
        {
          auto llog = spdlog::get(sname);
          if (ad.has("clear"))
            {
              try
                {
                  auto &mls = (*hit).second;
                  visitor_mllib::clear(mls, ad);
                }
              catch (MLLibBadParamException &e)
                {
                  llog->error("mllib bad param: {}", e.what());
                  throw;
                }
              catch (MLLibInternalException &e)
                {
                  llog->error("mllib internal error: {}", e.what());
                  throw;
                }
              catch (...)
                {
                  llog->error(
                      "delete service call failed: {}",
                      boost::current_exception_diagnostic_information());
                  throw;
                }
            }
          _mlservices.erase(hit);
          return true;
        }
      auto llog = spdlog::get("api");
      llog->error("cannot find service for removal");
      return false;
    }

    /**
     * \brief get a service position as iterator
     * @param sname service name
     * @return service position, end of container if not found
     */
    std::unordered_map<std::string, mls_variant_type>::iterator
    get_service_it(const std::string &sname)
    {
      std::unordered_map<std::string, mls_variant_type>::iterator hit;
      if ((hit = _mlservices.find(sname)) != _mlservices.end())
        return hit;
      return _mlservices.end();
    }

    /**
     * \brief checks whether a service exists
     * @param sname service name
     * return true if service exists, false otherwise
     */
    bool service_exists(const std::string &sname)
    {
      auto hit = get_service_it(sname);
      if (hit == _mlservices.end())
        return false;
      return true;
    }

    /**
     * \brief train a statistical model using a service
     * @param ad root data object
     * @param sname service name
     * @param out output data object
     */
    int train(const APIData &ad, const std::string &sname, APIData &out)
    {
      std::chrono::time_point<std::chrono::system_clock> tstart
          = std::chrono::system_clock::now();
      auto llog = spdlog::get(sname);
      int status = 0;
      try
        {
          auto hit = get_service_it(sname);
          auto &mls = (*hit).second;
          status = visitor_mllib::train_job(mls, ad, out);
        }
      catch (InputConnectorBadParamException &e)
        {
          llog->error("mllib bad param: {}", e.what());
          throw;
        }
      catch (MLLibBadParamException &e)
        {
          llog->error("mllib bad param: {}", e.what());
          throw;
        }
      catch (MLLibInternalException &e)
        {
          llog->error("mllib internal error: {}", e.what());
          throw;
        }
      catch (...)
        {
          llog->error("training call failed: {}",
                      boost::current_exception_diagnostic_information());
          throw;
        }

      if (ad.has("async") && ad.get("async").get<bool>())
        {
          out.add("job", status); // status holds the job id...
          out.add("status", std::string("running"));
          return 0; // status is OK i.e. the job has started.
        }
      else
        {
          std::chrono::time_point<std::chrono::system_clock> tstop
              = std::chrono::system_clock::now();
          double elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                               tstop - tstart)
                               .count();
          out.add("time", elapsed);
          return status;
        }
    }

    /**
     * \brief access to training job status
     * @param ad root data object
     * @param sname service name
     * @param out output data object
     */
    int train_status(const APIData &ad, const std::string &sname, APIData &out)
    {
      try
        {
          auto hit = get_service_it(sname);
          auto &mls = (*hit).second;
          return visitor_mllib::training_job_status(mls, ad, out);
        }
      catch (...)
        {
          auto llog = spdlog::get(sname);
          llog->error("training status call failed: {}",
                      boost::current_exception_diagnostic_information());
          throw;
        }
    }

    /**
     * \brief kills a training job
     * @param ad root data object
     * @param sname service name
     * @param output data object
     */
    int train_delete(const APIData &ad, const std::string &sname, APIData &out)
    {
      try
        {
          auto hit = get_service_it(sname);
          auto &mls = (*hit).second;
          return visitor_mllib::training_job_delete(mls, ad, out);
        }
      catch (...)
        {
          auto llog = spdlog::get(sname);
          llog->error("training delete call failed: {}",
                      boost::current_exception_diagnostic_information());
          throw;
        }
    }

    /**
     * \brief prediction from statistical model
     * @param ad root data object
     * @param sname service name
     * @return output DTO
     */
    oatpp::Object<DTO::PredictBody> predict(const APIData &ad_in,
                                            const std::string &sname,
                                            const bool &chain = false)
    {
      std::chrono::time_point<std::chrono::system_clock> tstart
          = std::chrono::system_clock::now();

      auto llog = spdlog::get(sname);
      oatpp::Object<DTO::PredictBody> pred_dto;
      try
        {
          auto hit = get_service_it(sname);
          auto &mllib = (*hit).second;

          // check for resource in data field
          std::vector<std::string> data_vec;
          if (ad_in.has("dto"))
            {
              auto any = ad_in.get("dto").get<oatpp::Any>();

              oatpp::Object<DTO::ServicePredict> predict_dto(
                  std::static_pointer_cast<typename DTO::ServicePredict>(
                      any->ptr));
              if (!predict_dto->data->empty())
                {
                  for (auto &val : *predict_dto->data)
                    data_vec.push_back(val);
                }
            }
          else
            {
              if (ad_in.has("data"))
                data_vec = ad_in.get("data").get<std::vector<std::string>>();
            }

          auto res_infos = oatpp::Vector<
              oatpp::Object<DTO::ResourceResponseBody>>::createShared();

          for (const auto &data_uri : data_vec)
            {
              auto rit = _resources.find(data_uri);
              if (rit != _resources.end())
                {
                  auto res_info = DTO::ResourceResponseBody::createShared();
                  visitor_resources::apply(
                      rit->second, const_cast<APIData &>(ad_in), res_info);
                  res_infos->push_back(res_info);
                }
            }

          // predict call
          pred_dto = visitor_mllib::predict_job(mllib, ad_in, chain);

          // update result with resource info
          if (!res_infos->empty())
            pred_dto->resources = res_infos;
        }
      catch (InputConnectorBadParamException &e)
        {
          llog->error("mllib bad param: {}", e.what());
          throw;
        }
      catch (MLLibBadParamException &e)
        {
          llog->error("mllib bad param: {}", e.what());
          throw;
        }
      catch (MLLibInternalException &e)
        {
          llog->error("mllib internal error: {}", e.what());
          throw;
        }
      catch (MLServiceLockException &e)
        {
          llog->error("mllib lock error: {}", e.what());
          throw;
        }
      catch (...)
        {
          llog->error("prediction call failed: {}",
                      boost::current_exception_diagnostic_information());
          throw;
        }
      std::chrono::time_point<std::chrono::system_clock> tstop
          = std::chrono::system_clock::now();
      double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           tstop - tstart)
                           .count();
      pred_dto->time = elapsed;
      return pred_dto;
    }

    int chain_service(const std::string &cname,
                      const std::shared_ptr<spdlog::logger> &chain_logger,
                      APIData &adc, ChainData &cdata,
                      const std::string &pred_id,
                      std::vector<std::string> &meta_uris,
                      std::vector<std::string> &index_uris,
                      const std::string &parent_id, const int chain_pos,
                      int &npredicts)
    {
      std::string sname = adc.get("service").get<std::string>();
      chain_logger->info("[" + std::to_string(chain_pos)
                         + "] / executing predict on service " + sname);

      // need to check that service exists
      if (!service_exists(sname))
        {
          spdlog::drop(cname);
          throw ServiceNotFoundException("Service " + sname
                                         + " does not exist");
        }

      // if not first predict call in the chain, need to setup the input
      // data!
      if (chain_pos != 0)
        {
          // take data from the previous action
          APIData act_data = cdata.get_action_data(parent_id);
          if (act_data.empty())
            {
              spdlog::drop(cname);
              throw InputConnectorBadParamException(
                  "no action data for action id " + parent_id);
            }
          if (act_data.has("data"))
            {
              adc.add("data",
                      act_data.get("data")
                          .get<std::vector<std::string>>()); // action output
                                                             // data must be
                                                             // string for now
                                                             // (more types to
                                                             // be supported /
                                                             // auto-detected)
            }
#ifdef USE_CUDA_CV
          else if (act_data.has("data_cuda_img"))
            {
              adc.add("data_raw_img_cuda",
                      act_data.get("data_cuda_img")
                          .get<std::vector<cv::cuda::GpuMat>>());
            }
#endif
          else if (act_data.has("data_raw_img")) // raw images
            {
              adc.add(
                  "data_raw_img",
                  act_data.get("data_raw_img").get<std::vector<cv::Mat>>());
            }
          adc.add("ids",
                  act_data.get("cids")
                      .get<std::vector<std::string>>()); // chain ids of
                                                         // processed elements
          adc.add("meta_uris", meta_uris);
          adc.add("index_uris", index_uris);
        }
      else
        {
          cdata._first_id = pred_id;
        }

      oatpp::Object<DTO::PredictBody> pred_dto;
      try
        {
          pred_dto = predict(adc, sname, true);
        }
      catch (...)
        {
          spdlog::drop(cname);
          throw;
        }

      int classes_size = 0;
      int vals_size = 0;
      std::vector<std::string> nmeta_uris;
      std::vector<std::string> nindex_uris;

      auto predictions = pred_dto->predictions;
      if (predictions->empty())
        {
          chain_logger->info("[" + std::to_string(chain_pos)
                             + "]  no predictions");
          return 1;
        }

      for (size_t j = 0; j < predictions->size(); j++)
        {
          auto pred = predictions->at(j);
          size_t npred_classes
              = std::max(pred->classes != nullptr ? pred->classes->size() : 0,
                         pred->vector != nullptr ? pred->vector->size() : 0);

          classes_size += npred_classes;
          vals_size += static_cast<int>(pred->vals != nullptr)
                       + static_cast<int>(pred->images->size());

          if (chain_pos == 0) // first call's response contains uniformized
                              // top level URIs.
            {
              for (size_t k = 0; k < npred_classes; k++)
                {
                  nmeta_uris.push_back(pred->uri);
                  if (pred->index_uri)
                    nindex_uris.push_back(pred->index_uri);
                }
            }
          else // update meta uris to batch size at the current level
               // of the chain
            {
              for (size_t k = 0; k < npred_classes; k++)
                {
                  nmeta_uris.push_back(meta_uris.at(j));
                  if (!index_uris.empty())
                    nindex_uris.push_back(index_uris.at(j));
                }
            }
        }

      meta_uris = nmeta_uris;
      index_uris = nindex_uris;

      if (!classes_size && !vals_size)
        {
          chain_logger->info("[" + std::to_string(chain_pos)
                             + "] / no result from prediction");
          cdata.add_model_data(
              pred_id,
              DTO::PredictBody::createShared()); // store empty model output
          return 1;
        }

      ++npredicts;

      // store model output
      cdata.add_model_data(pred_id, pred_dto);

      return 0;
    }

    int chain_action(const std::shared_ptr<spdlog::logger> &chain_logger,
                     APIData &adc, ChainData &cdata, const int &chain_pos,
                     const std::string &prec_pred_id)
    {
      std::string action_type
          = adc.getobj("action").get("type").get<std::string>();

      oatpp::Object<DTO::PredictBody> prev_data
          = cdata.get_model_data(prec_pred_id);
      if (prev_data->predictions->empty())
        {
          // no prediction to work from
          chain_logger->info("no prediction to act on");
          return 1;
        }

      // call chain action factory
      chain_logger->info("[" + std::to_string(chain_pos)
                         + "] / executing action " + action_type);
      ChainActionFactory caf(adc);
      caf.apply_action(action_type, prev_data, cdata, chain_logger);

      // replace prev_data in cdata for prec_pred_id
      cdata.add_model_data(prec_pred_id, prev_data);

      auto predictions = prev_data->predictions;
      if (predictions->empty())
        {
          // no prediction to work from
          chain_logger->info("no prediction to act on after applying action "
                             + action_type);
          return 1;
        }

      // check that there are predictions
      int classes_size = 0;
      int vals_size = 0;
      for (size_t i = 0; i < predictions->size(); i++)
        {
          auto pred = predictions->at(i);
          size_t npred_classes
              = std::max(pred->classes != nullptr ? pred->classes->size() : 0,
                         pred->vector != nullptr ? pred->vector->size() : 0);
          classes_size += npred_classes;
          vals_size += static_cast<int>(pred->vals != nullptr)
                       + static_cast<int>(pred->images->size());
        }

      if (classes_size == 0 && vals_size == 0)
        {
          chain_logger->info("[" + std::to_string(chain_pos)
                             + "] / no result after applying action "
                             + action_type);
          return 1;
        }

      return 0;
    }

    oatpp::Object<DTO::ChainBody> chain(const APIData &ad,
                                        const std::string &cname)
    {
      oatpp::Object<DTO::ChainBody> chain_dto;
      try
        {
          auto chain_logger = DD_SPDLOG_LOGGER(cname);

          std::chrono::time_point<std::chrono::system_clock> tstart
              = std::chrono::system_clock::now();

          // - iterate chain of calls
          // - if predict call, use the visitor to execute it
          //      - this requires storing output into mlservice object /
          //      have a flag for telling the called service it's part of a
          //      chain
          // - if action call, execute the generic code for it
          std::vector<APIData> ad_calls = ad.getobj("chain").getv("calls");
          chain_logger->info("number of calls="
                             + std::to_string(ad_calls.size()));

          // debug
          /*std::vector<std::string> ckeys = ad.list_keys();
            for (auto s: ckeys)
            std::cerr << s << std::endl;*/
          // debug

          ChainData cdata;
          std::vector<std::string> meta_uris;
          std::vector<std::string> index_uris;
          std::unordered_map<std::string, std::vector<std::string>>
              um_meta_uris;
          std::unordered_map<std::string, std::vector<std::string>>
              um_index_uris;
          int npredicts = 0;
          std::string prec_pred_id;
          std::string prec_action_id;
          int aid = 0;
          for (size_t i = 0; i < ad_calls.size(); i++)
            {
              APIData adc = ad_calls.at(i);
              if (adc.has("service"))
                {
                  std::string pred_id;
                  if (adc.has("id"))
                    pred_id = adc.get("id").get<std::string>();
                  else
                    pred_id = std::to_string(i);

                  std::string parent_id;
                  if (adc.has("parent_id"))
                    parent_id = adc.get("parent_id").get<std::string>();
                  else
                    parent_id = prec_action_id;

                  auto hit = um_meta_uris.find(parent_id);
                  if (hit != um_meta_uris.end())
                    meta_uris = (*hit).second;
                  hit = um_index_uris.find(parent_id);
                  if (hit != um_index_uris.end())
                    index_uris = (*hit).second;
                  cdata.add_model_sname(pred_id,
                                        adc.get("service").get<std::string>());
                  if (chain_service(cname, chain_logger, adc, cdata, pred_id,
                                    meta_uris, index_uris, parent_id, i,
                                    npredicts))
                    break;
                  prec_pred_id = pred_id;
                }
              else if (adc.has("action"))
                {
                  if (chain_action(chain_logger, adc, cdata, i, prec_pred_id))
                    break;
                  if (adc.has("id"))
                    prec_action_id = adc.get("id").get<std::string>();
                  else
                    prec_action_id = std::to_string(aid);
                  um_meta_uris.insert(
                      std::pair<std::string, std::vector<std::string>>(
                          prec_action_id, meta_uris));
                  um_index_uris.insert(
                      std::pair<std::string, std::vector<std::string>>(
                          prec_action_id, index_uris));
                  ++aid;
                }
            }

          // producing a nested output
          chain_dto = cdata.nested_chain_output();

          std::chrono::time_point<std::chrono::system_clock> tstop
              = std::chrono::system_clock::now();
          double elapsed
              = std::chrono::duration_cast<std::chrono::milliseconds>(tstop
                                                                      - tstart)
                    .count();
          chain_dto->time = elapsed;
        }
      catch (...)
        {
          spdlog::drop(cname);
          throw;
        }
      spdlog::drop(cname);

      return chain_dto;
    }

    // =====
    //
    // with DTO input

    oatpp::Object<DTO::PredictBody>
    predict(const std::string &sname,
            const oatpp::Object<DTO::ServicePredict> &predict_dto)
    {
      APIData pred_in;
      pred_in.add("dto", predict_dto);
      return predict(pred_in, sname);
    }

    int chain_service(const std::string &cname,
                      const std::shared_ptr<spdlog::logger> &chain_logger,
                      oatpp::Object<DTO::ChainCall> &call_dto,
                      ChainData &cdata, const std::string &pred_id,
                      std::vector<std::string> &meta_uris,
                      std::vector<std::string> &index_uris,
                      const std::string &parent_id, const int chain_pos,
                      int &npredicts)
    {
      std::string sname = call_dto->service;
      chain_logger->info("[" + std::to_string(chain_pos)
                         + "] / executing predict on service " + sname);

      // need to check that service exists
      if (!service_exists(sname))
        {
          spdlog::drop(cname);
          throw ServiceNotFoundException("Service " + sname
                                         + " does not exist");
        }

      // if not first predict call in the chain, need to setup the input
      // data!
      if (chain_pos != 0)
        {
          // take data from the previous action
          APIData act_data = cdata.get_action_data(parent_id);
          if (act_data.empty())
            {
              spdlog::drop(cname);
              throw InputConnectorBadParamException(
                  "no action data for action id " + parent_id);
            }
          if (act_data.has("data"))
            {
              // action output data must be string for now (more types to be
              // supported / auto-detected)
              for (auto d :
                   act_data.get("data").get<std::vector<std::string>>())
                {
                  call_dto->data->push_back(d.c_str());
                }
            }
#ifdef USE_CUDA_CV
          else if (act_data.has("data_cuda_img"))
            {
              call_dto->_data_raw_img_cuda
                  = act_data.get("data_cuda_img")
                        .get<std::vector<cv::cuda::GpuMat>>();
            }
#endif
          else if (act_data.has("data_raw_img")) // raw images
            {
              call_dto->_data_raw_img
                  = act_data.get("data_raw_img").get<std::vector<cv::Mat>>();
            }
          call_dto->_ids
              = act_data.get("cids")
                    .get<std::vector<std::string>>(); // chain ids of
                                                      // processed elements
          call_dto->_meta_uris = meta_uris;
          call_dto->_index_uris = index_uris;
        }
      else
        {
          cdata._first_id = pred_id;
        }

      call_dto->_chain = true;

      APIData pred_in;
      pred_in.add("dto", call_dto);
      oatpp::Object<DTO::PredictBody> pred_dto;
      try
        {
          pred_dto = predict(pred_in, sname, true);
        }
      catch (...)
        {
          spdlog::drop(cname);
          throw;
        }

      int classes_size = 0;
      int vals_size = 0;
      std::vector<std::string> nmeta_uris;
      std::vector<std::string> nindex_uris;

      auto predictions = pred_dto->predictions;
      if (predictions->empty())
        {
          chain_logger->info("[" + std::to_string(chain_pos)
                             + "]  no predictions");
          return 1;
        }

      for (size_t j = 0; j < predictions->size(); j++)
        {
          auto pred = predictions->at(j);
          size_t npred_classes
              = std::max(pred->classes != nullptr ? pred->classes->size() : 0,
                         pred->vector != nullptr ? pred->vector->size() : 0);
          classes_size += npred_classes;
          vals_size += static_cast<int>(pred->vals != nullptr)
                       + static_cast<int>(pred->images->size());

          if (chain_pos == 0) // first call's response contains uniformized
                              // top level URIs.
            {
              for (size_t k = 0; k < npred_classes; k++)
                {
                  nmeta_uris.push_back(pred->uri);
                  if (pred->index_uri)
                    nindex_uris.push_back(pred->index_uri);
                }
            }
          else // update meta uris to batch size at the current level
               // of the chain
            {
              for (size_t k = 0; k < npred_classes; k++)
                {
                  nmeta_uris.push_back(meta_uris.at(j));
                  if (!index_uris.empty())
                    nindex_uris.push_back(index_uris.at(j));
                }
            }
        }
      meta_uris = nmeta_uris;
      index_uris = nindex_uris;

      if (!classes_size && !vals_size)
        {
          chain_logger->info("[" + std::to_string(chain_pos)
                             + "] / no result from prediction");
          cdata.add_model_data(
              pred_id,
              DTO::PredictBody::createShared()); // store empty model output
          return 1;
        }

      ++npredicts;

      // store model output
      cdata.add_model_data(pred_id, pred_dto);
      return 0;
    }

    int chain_action(const std::shared_ptr<spdlog::logger> &chain_logger,
                     oatpp::Object<DTO::ChainCall> call_dto, ChainData &cdata,
                     const int &chain_pos, const std::string &prec_pred_id)
    {
      std::string action_type = call_dto->action->type;

      oatpp::Object<DTO::PredictBody> prev_data
          = cdata.get_model_data(prec_pred_id);
      auto predictions = prev_data->predictions;
      if (predictions->empty())
        {
          // no prediction to work from
          chain_logger->info("no prediction to act on");
          return 1;
        }

      // call chain action factory
      chain_logger->info("[" + std::to_string(chain_pos)
                         + "] / executing action " + action_type);
      ChainActionFactory caf(call_dto);
      caf.apply_action(action_type, prev_data, cdata, chain_logger);

      // replace prev_data in cdata for prec_pred_id
      cdata.add_model_data(prec_pred_id, prev_data);

      // TODO see if it is necessary
      predictions = prev_data->predictions;
      if (predictions->empty())
        {
          // no prediction to work from
          chain_logger->info("no prediction to act on after applying action "
                             + action_type);
          return 1;
        }

      int classes_size = 0;
      int vals_size = 0;
      for (size_t i = 0; i < predictions->size(); i++)
        {
          auto pred = predictions->at(i);
          size_t npred_classes
              = std::max(pred->classes != nullptr ? pred->classes->size() : 0,
                         pred->vector != nullptr ? pred->vector->size() : 0);
          classes_size += npred_classes;
          vals_size += static_cast<int>(pred->vals != nullptr);
        }

      if (!classes_size && !vals_size)
        {
          chain_logger->info("[" + std::to_string(chain_pos)
                             + "] / no result after applying action "
                             + action_type);
          return 1;
        }

      return 0;
    }

    oatpp::Object<DTO::ChainBody>
    chain(oatpp::Object<DTO::ServiceChain> input_dto, const std::string &cname)
    {
      oatpp::Object<DTO::ChainBody> out_dto;
      try
        {
          auto chain_logger = DD_SPDLOG_LOGGER(cname);

          std::chrono::time_point<std::chrono::system_clock> tstart
              = std::chrono::system_clock::now();

          // - iterate chain of calls
          // - if predict call, use the visitor to execute it
          //      - this requires storing output into mlservice object /
          //      have a flag for telling the called service it's part of a
          //      chain
          // - if action call, execute the generic code for it
          auto calls_vec = input_dto->chain->calls;
          chain_logger->info("number of calls="
                             + std::to_string(calls_vec->size()));

          ChainData cdata;
          std::vector<std::string> meta_uris;
          std::vector<std::string> index_uris;
          std::unordered_map<std::string, std::vector<std::string>>
              um_meta_uris;
          std::unordered_map<std::string, std::vector<std::string>>
              um_index_uris;
          int npredicts = 0;
          std::string prec_pred_id;
          std::string prec_action_id;
          int aid = 0;

          for (size_t i = 0; i < calls_vec->size(); i++)
            {
              auto call = calls_vec->at(i);
              std::string call_id = call->id != nullptr ? std::string(call->id)
                                                        : std::to_string(i);
              std::string parent_id = call->parent_id != nullptr
                                          ? std::string(call->parent_id)
                                          : prec_action_id;

              if (call->service != nullptr)
                {
                  if (call->action != nullptr)
                    {
                      throw ChainBadParamException(
                          "Chain call #" + call_id
                          + " defines both a service and an action");
                    }

                  auto hit = um_meta_uris.find(parent_id);
                  if (hit != um_meta_uris.end())
                    meta_uris = (*hit).second;
                  hit = um_index_uris.find(parent_id);
                  if (hit != um_index_uris.end())
                    index_uris = (*hit).second;
                  cdata.add_model_sname(call_id, call->service);
                  if (chain_service(cname, chain_logger, call, cdata, call_id,
                                    meta_uris, index_uris, parent_id, i,
                                    npredicts))
                    break;
                  prec_pred_id = call_id;
                }
              else if (call->action != nullptr)
                {
                  if (chain_action(chain_logger, call, cdata, i, prec_pred_id))
                    break;

                  prec_action_id = call_id;
                  um_meta_uris.insert(
                      std::pair<std::string, std::vector<std::string>>(
                          prec_action_id, meta_uris));
                  um_index_uris.insert(
                      std::pair<std::string, std::vector<std::string>>(
                          prec_action_id, index_uris));
                  ++aid;
                }
              else
                {
                  throw ChainBadParamException(
                      "no services nor action found in chain call #"
                      + std::to_string(i));
                }
            }

          // producing a nested output
          if (npredicts > 1)
            out_dto = cdata.nested_chain_output();
          else
            {
              auto first_model_dto = cdata.get_model_data(cdata._first_id);
              out_dto = DTO::ChainBody::createShared();

              for (auto &pred : *first_model_dto->predictions)
                {
                  out_dto->predictions->push_back(
                      oatpp_utils::dtoToUFields(pred));
                }
            }

          std::chrono::time_point<std::chrono::system_clock> tstop
              = std::chrono::system_clock::now();
          double elapsed
              = std::chrono::duration_cast<std::chrono::milliseconds>(tstop
                                                                      - tstart)
                    .count();
          out_dto->time = elapsed;
        }
      catch (...)
        {
          spdlog::drop(cname);
          throw;
        }
      spdlog::drop(cname);
      return out_dto;
    }

    oatpp::Object<DTO::ResourceResponse>
    create_resource(const std::string &resource_name,
                    oatpp::Object<DTO::Resource> resource_data)
    {
      std::unordered_map<std::string, res_variant_type>::const_iterator hit;
      if ((hit = _resources.find(resource_name)) != _resources.end())
        {
          throw ResourceForbiddenException("Resource already exists");
        }

      res_variant_type res
          = ResourceFactory::create(resource_name, resource_data);

      auto llog = spdlog::get(resource_name);
      auto response = DTO::ResourceResponse::createShared();
      response->body = DTO::ResourceResponseBody::createShared();
      try
        {
          visitor_resources::init(res, resource_data);
          // get resource info
          visitor_resources::get_info(res, response->body);

          std::lock_guard<std::mutex> lock(_resources_mtx);
          _resources.insert(std::pair<std::string, res_variant_type>(
              resource_name, std::move(res)));
        }
      catch (...)
        {
          llog->error("Resource creation call failed: {}",
                      boost::current_exception_diagnostic_information());
          throw;
        }

      return response;
    }

    oatpp::Object<DTO::ResourceResponse>
    get_resource(const std::string &resource_name)
    {
      auto llog = spdlog::get(resource_name);
      auto it = _resources.find(resource_name);

      if (it == _resources.end())
        throw ResourceNotFoundException("Resource with name " + resource_name
                                        + " does not exist");
      auto response = DTO::ResourceResponse::createShared();
      response->body = DTO::ResourceResponseBody::createShared();
      try
        {
          visitor_resources::get_info(it->second, response->body);
        }
      catch (...)
        {
          llog->error("Resource creation call failed: {}",
                      boost::current_exception_diagnostic_information());
          throw;
        }
      return response;
    }

    void delete_resource(const std::string &resource_name)
    {
      auto llog = spdlog::get(resource_name);
      auto it = _resources.find(resource_name);

      if (it == _resources.end())
        throw ResourceNotFoundException("Resource with name " + resource_name
                                        + " does not exist");

      _resources.erase(it);
    }

    oatpp::Object<DTO::StreamResponse>
    create_stream(std::string stream_name,
                  oatpp::Object<DTO::Stream> stream_data)
    {
      (void)stream_name;
      (void)stream_data;
      auto response = DTO::StreamResponse::createShared();
      return response;
    }

    oatpp::Object<DTO::StreamResponse>
    get_stream_info(const std::string &stream_name)
    {
      (void)stream_name;
      auto response = DTO::StreamResponse::createShared();
      return response;
    }

    int delete_stream(const std::string stream_name)
    {
      (void)stream_name;
      return 200;
    }

    std::unordered_map<std::string, mls_variant_type>
        _mlservices; /**< container of instanciated services. */

    std::unordered_map<std::string, res_variant_type>
        _resources; /**< container of instanciated resources */

  protected:
    std::mutex _mlservices_mtx; /**< mutex around adding/removing services. */
    std::mutex _resources_mtx;  /**< mutex around adding/removing resources. */
  };
}

#endif
