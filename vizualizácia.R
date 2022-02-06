library(ggplot2)

data1<-read.table("Ito_1.txt",sep=',',header=TRUE)
g1<-ggplot(data1, aes(x=x, y=y, group=typ)) + geom_line() + labs(x="x", y="y") 

data2<-read.table("Ito_2.txt",sep=',',header=TRUE)
g2<-ggplot(data2, aes(x=x, y=y, group=typ)) + geom_line() + labs(x="x", y="y")

g1
g2

