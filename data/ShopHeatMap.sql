/*
 Navicat Premium Data Transfer

 Source Server         : shopheat
 Source Server Type    : MySQL
 Source Server Version : 50732
 Source Host           : 192.168.0.167:4000
 Source Schema         : ShopHeatMap

 Target Server Type    : MySQL
 Target Server Version : 50732
 File Encoding         : 65001

 Date: 04/02/2021 00:31:26
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for AreaGoods
-- ----------------------------

CREATE TABLE IF NOT EXISTS `AreaGoods`  (
  `AreaID` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `GoodsID` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for CameraAddrs
-- ----------------------------

CREATE TABLE IF NOT EXISTS `CameraAddrs`  (
  `CameraIP` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `CameraID` int(255) NOT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for CameraCoord
-- ----------------------------

CREATE TABLE IF NOT EXISTS `CameraCoord`  (
  `CoordNo` int(11) NOT NULL AUTO_INCREMENT,
  `CameraNo` int(11) NOT NULL,
  `CameraCoord` json NULL COMMENT '{position:[]}',
  `MapCoord` json NULL COMMENT '{position:[]}',
  PRIMARY KEY (`CoordNo`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 300 CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for Goods
-- ----------------------------

CREATE TABLE IF NOT EXISTS `Goods`  (
  `ID` varchar(255) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL,
  `Price` decimal(10, 2) NULL DEFAULT NULL,
  `Pic` text CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL,
  `Name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `Comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for HouseMap
-- ----------------------------

CREATE TABLE IF NOT EXISTS `HouseMap`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Polygon` json NULL,
  `ObjectType` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `Name` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 69 CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for PeoplePosition
-- ----------------------------

CREATE TABLE IF NOT EXISTS `PeoplePosition`  (
  `Position` json NULL,
  `Time` datetime(6) NULL DEFAULT NULL,
  `ID` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;


SET FOREIGN_KEY_CHECKS = 1;
