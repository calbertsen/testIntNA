\name{testIntNA-package}
\alias{testIntNA-package}
\alias{testIntNA}
\docType{package}
\title{
\packageTitle{testIntNA}
}
\description{
\packageDescription{testIntNA}
}
\details{

The DESCRIPTION file:
\packageDESCRIPTION{testIntNA}
\packageIndices{testIntNA}
bla bla
}
\author{
\packageAuthor{testIntNA}

Maintainer: \packageMaintainer{testIntNA}
}
\references{
Nothing here
}
\keyword{ package }
\seealso{
TMB
}
\examples{
library(testIntNA)
library(TMB)
dat <- list(a = NA_integer_,
b = rep(NA_integer_,5),
d = matrix(NA_integer_,3,3),
e = array(NA_integer_,dim=c(5,5,5))
)
obj <- MakeADFun(dat,list(x=0), DLL="testIntNA")
obj$fn()
obj$report()
}
