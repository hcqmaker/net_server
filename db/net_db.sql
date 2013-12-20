/*
SQLyog Ultimate v8.32 
MySQL - 5.5.10-log : Database - net_db
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`net_db` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `net_db`;

/*Table structure for table `g_user` */

DROP TABLE IF EXISTS `g_user`;

CREATE TABLE `g_user` (
  `id` bigint(64) NOT NULL AUTO_INCREMENT,
  `user_id` int(32) DEFAULT NULL,
  `user_name` varchar(256) DEFAULT '',
  `level` int(32) DEFAULT '0',
  `gender` int(32) DEFAULT '0' COMMENT '0/1 female/male',
  `role_id` int(32) DEFAULT '0',
  `scene_id` int(32) DEFAULT '0',
  `x` int(32) DEFAULT '0',
  `y` int(32) DEFAULT '0',
  `user_pwd` varchar(8) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `g_user` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
