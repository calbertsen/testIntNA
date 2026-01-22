a <- function() {
    a0 <- rep(NA_integer_,5)
    .Call("testValues",list(vec=a0), PACKAGE="testIntNA")

}


