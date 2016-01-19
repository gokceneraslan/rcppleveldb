## RcppLevelDB [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html)

RcppLevelDB is a Rcpp based LevelDB binding for R

Support for iterators was added by Gabe Rudy.

### Dependencies

- [leveldb](https://github.com/google/leveldb), leveldb C++ library, e.g. via [libleveldb-dev](https://packages.debian.org/sid/libleveldb-dev) on Debian or Ubuntu
- [Rcpp](https://github.com/RcppCore/Rcpp) for seamless R and C++ integration
- [RApiSerialize](https://github.com/eddelbuettel/rapiserialize) for C-level serialization from the R API

### Getting Started

```r
library(RcppLevelDB)

ldb <- new(LevelDB, 'my-leveldb') #creates the DB, if not exists

ldb$Put('key1', list(a=c(1,2,3), b='sometext', c=42))
#>[1] TRUE
ldb$Put('anotherkey', 'somevalue')
#>[1] TRUE

#vectorized Get & Delete operations
ldb$Get(c('key1', 'anotherkey'))
#>[[1]]
#>[[1]]$a
#>[1] 1 2 3
#>
#>[[1]]$b
#>[1] "sometext"
#>
#>[[1]]$c
#>[1] 42
#>
#>
#>[[2]]
#>[1] "somevalue"
#>

#Iteration
ldb$StartIteration()
#>[1] TRUE
ldb$IterNext()
#>[[1]]
#>[1] "anotherkey"
#>
#>[[2]]
#>[1] "somevalue"
#>
> ldb$IterNext()
#>[[1]]
#>[1] "key1"
#>
#>[[2]]
#>[[2]]$a
#>[1] 1 2 3
#>
#>[[2]]$b
#>[1] "sometext"
#>
#>[[2]]$c
#>[1] 42
#>
ldb$IterNext()
#>[[1]]
#>[1] NA
#>
#> 

#Deletion
ldb$Delete(c('key1', 'anotherkey'))
#>[1] TRUE TRUE
ldb$Get(c('key1', 'anotherkey'))
#>[[1]]
#>[1] NA
#>
#>[[2]]
#>[1] NA
#>
rm(ldb) #to close the database safely
gc() # remove all handles
```

### TODO

- Expose `Options/ReadOptions/WriteOptions` structs to R
- Add default arguments to constructor and Put/Get/Delete methods e.g. `create_if_missing=TRUE`, `sync=FALSE` etc.
- Add support for BloomFilter policies, WriteBatch, and Iterators
- Add RepairDB and DestroyDB functions
- Something like `Structured Get` operation: create a data.frame out of values of the given query keys, when the values have a pre-defined structure
- [plyvel](https://plyvel.readthedocs.org/en/latest/api.html) is a nice leveldb binding example to get some influence regarding new features

### Author

Gökcen Eraslan, based on RcppRedis by Dirk Eddelbuettel

### License

GPL (>= 2)
