a <- function() {
    a0 <- array(NA_real_,dim = c(5,5))
    .Call("testValues",list(vec=a0), PACKAGE="testIntNA")

}


