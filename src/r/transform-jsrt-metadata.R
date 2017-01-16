library(stringr)

setwd("C:/toil/radiology-cv/src/r")

ln_list <- read.table("../../data/CLNDAT_EN.txt", sep="\t")
ln_list = ln_list[, -c(9, 10, 11, 12)]
names(ln_list) <- c("filename", "subtlety", "size_mm", "age", "sex", "x_coord", "y_coord", "malignancy")
ln <- as.data.frame(ln_list)
ln$subtlety <- factor(ln$subtlety)
ln$sex <- factor(ln$sex)
ln$malignant <- ln$malignancy == "malignant"
ln$nodule <- TRUE
ln <- ln[, !(names(ln) %in% c("malignancy"))]
rm(ln_list)

nn_raw <- read.table("../../data/CNNDAT_EN.txt", sep="\t")
names(nn_raw) <- c("data")
nn <- as.data.frame(str_split_fixed(nn_raw$data, "( )+", 4))
names(nn) <- c("filename", "age", "sex", "nodule")
nn$sex = factor(nn$sex)
nn$subtlety <- as.factor(NA)
nn$size_mm <- 0
nn$x_coord <- -1
nn$y_coord <- -1
nn$malignant <- FALSE
nn$nodule <- nn$nodule != "non-nodule"
rm(nn_raw)

xray_metadata <- rbind(ln, nn)

write.csv(xray_metadata, file = "../../data/xray_metadata.csv", row.names=FALSE)
