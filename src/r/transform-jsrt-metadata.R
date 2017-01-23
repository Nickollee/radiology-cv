library(stringr)

setwd("C:/toil/radiology-cv/src/r")

ln_list <- read.table("../../data/clinical/CLNDAT_EN.txt", sep="\t")
ln_list = ln_list[, -c(9, 10, 11, 12)]
names(ln_list) <- c("filename", "subtlety", "size_mm", "age", "sex", "x_coord", "y_coord", "malignancy")
ln <- as.data.frame(ln_list)
ln$malignant <- ln$malignancy == "malignant"
ln$nodule <- TRUE
ln <- ln[, !(names(ln) %in% c("malignancy"))]
rm(ln_list)

nn_raw <- read.table("../../data/clinical/CNNDAT_EN.txt", sep="\t")
names(nn_raw) <- c("data")
nn <- as.data.frame(str_split_fixed(nn_raw$data, "( )+", 4))
names(nn) <- c("filename", "age", "sex", "nodule")

nn$subtlety <- 0
nn$size_mm <- 0
nn$x_coord <- 0
nn$y_coord <- 0
nn$malignant <- FALSE
nn$nodule <- FALSE
rm(nn_raw)

xray_metadata <- rbind(ln, nn)
xray_metadata[ xray_metadata == "?" ] = NA

xray_metadata$sex <- xray_metadata$sex == 'male'
xray_metadata$filename <- as.character(xray_metadata$filename)


xray_metadata$age <- round(as.numeric(as.character(xray_metadata$age)), digits = 0)
xray_metadata[is.na(xray_metadata[,4]), 4] <- round(mean(xray_metadata[,4], na.rm = TRUE), digits = 0)

corrupted_imgs <- c("LN031", "LN037", "LN078", "LN083", "LN094", "LN117")
xray_metadata <- xray_metadata[!str_extract(xray_metadata$filename, "LN[0-9]{3}") %in% corrupted_imgs, ]

write.csv(xray_metadata, file = "../../data/clinical/xray_metadata.csv", row.names=FALSE)
