// -*- indent-tabs-mode: nil; tab-width: 4; c-indent-level: 4; c-basic-offset: 4; -*-
//
//  rleveldb -- Based on Redis.cpp file in RcppRedis project
//
//  Copyright (C) 2015 Gokcen Eraslan
//
//  rleveldb is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  rleveldb is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with rcppleveldb.  If not, see <http://www.gnu.org/licenses/>.


#include <Rcpp.h>
#include <leveldb/db.h>

#include <RApiSerializeAPI.h>   	// provides C API with serialization for R


// A simple and lightweight class -- with just a simple private member variable

class LevelDB {

private:

  leveldb::DB* _pldb;                // private pointer to leveldb

  // open LevelDB database
  void init(std::string dbfilename, leveldb::Options options)  {
    leveldb::Status status = leveldb::DB::Open(options, dbfilename, &_pldb);
    if (!status.ok())
      Rcpp::stop(std::string("LevelDB open error: ") + status.ToString());
  }

public:

  //LevelDB(std::string dbfilename, leveldb::Options options)  { init(dbfilename, options); }
  LevelDB(std::string dbfilename) {
    leveldb::Options options;
    options.create_if_missing = true;
    init(dbfilename, options);
  }

  ~LevelDB() {
    delete _pldb;
    _pldb = NULL;                // just to be on the safe side
  }

  bool Put(std::string key, SEXP s) {

    // if raw, use as is else serialize to raw
    Rcpp::RawVector x = (TYPEOF(s) == RAWSXP) ? s : serializeToRaw(s);

    leveldb::Slice sl((char *)x.begin(), (size_t) x.size());
    leveldb::Status stat = _pldb->Put(leveldb::WriteOptions(), key, sl);

    return stat.ok();
  }

  Rcpp::LogicalVector Delete(Rcpp::CharacterVector cv) {

    Rcpp::LogicalVector bv(cv.size());

    for (int i = 0; i < cv.size(); i++) {
      std::string key = Rcpp::as<std::string>(cv[i]);
      leveldb::Status stat = _pldb->Delete(leveldb::WriteOptions(), key);
      bv[i] = stat.ok();
    }

    return bv;
  }


  SEXP Get(Rcpp::CharacterVector cv) {

    Rcpp::List l(cv.size());
    SEXP ret;

    for (int i = 0; i < cv.size(); i++) {
      std::string value;
      std::string key = Rcpp::as<std::string>(cv[i]);
      leveldb::Status s = _pldb->Get(leveldb::ReadOptions(), key, &value);

      //if (!s.ok() && !returnNA) {
      //  Rcpp::stop(std::string("LevelDB get error: ") + s.ToString());
      //}
      //else if (!s.ok()) {
      if (!s.ok()) {
        ret = Rcpp::wrap(NA_LOGICAL);
      }
      else {
        Rcpp::RawVector x(value.size());
        std::copy( value.begin(), value.end(), x.begin() ) ;

        ret = unserializeFromRaw(x);
      }

      l[i] = ret;
    }

    if (cv.size() == 1)
      return l[0];
    else
      return l;
  }
};


RCPP_MODULE(LevelDB) {
  Rcpp::class_<LevelDB>("LevelDB")

  .constructor<std::string>("default constructor")
  //.constructor<std::string, leveldb::Options>("constructor with options")

  .method("Put", &LevelDB::Put, "Calls Put method of the LevelDB object, serializes internally")
  .method("Delete", &LevelDB::Delete, "Calls Delete method of the LevelDB object, serializes internally")
  .method("Get",  &LevelDB::Get,   "Vectorized Get method, deserializes internally")
  ;
}
