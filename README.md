## RcppLevelDB [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html)

RcppLevelDB is a Rcpp based LevelDB binding for R

### Dependencies

- [leveldb](https://github.com/google/leveldb), leveldb, eg via [libleveldb-dev](https://packages.debian.org/sid/libleveldb-dev) on Debian or Ubuntu
- [Rcpp](https://github.com/RcppCore/Rcpp) for seamless R and C++ integration
- [RApiSerialize](https://github.com/eddelbuettel/rapiserialize) for C-level serialization from the R API

### Getting Started

```
library(RcppLevelDB)

ldb <- new(LevelDB, 'my-leveldb') #creates the DB, if not exists

ldb$Put('key1', list(a=c(1,2,3), b='sometext', c=42))
ldb$Put('anotherkey', 'somevalue')

#vectorized Get & Delete operations
ldb$Get(c('key1', 'anotherkey'))
ldb$Delete(c('key1', 'anotherkey'))
ldb$Get(c('key1', 'anotherkey'))

rm(ldb) #to close the database safely
```

### Author

Gökcen Eraslan, based on RcppRedis by Dirk Eddelbuettel

### License

GPL (>= 2)
