# MySQL Navigator Xport
# Database: openanno
# 

#
# Table structure for table 'gameMembers'
#

DROP TABLE IF EXISTS gameMembers;
CREATE TABLE `gameMembers` (
  `GameID` int(10) unsigned NOT NULL,
  `UserID` int(10) unsigned NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

#
# Table structure for table 'games'
#

DROP TABLE IF EXISTS `games`;
CREATE TABLE `games` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `location` varchar(100) default 'N/A',
  `name` varchar(50) default 'N/A',
  `host` varchar(15) NOT NULL,
  `ping` mediumint(8) unsigned NOT NULL,
   PRIMARY KEY  (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

#
# Table structure for table 'users'
#

DROP TABLE IF EXISTS users;
CREATE TABLE `users` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `username` varchar(30) NOT NULL,
  `password` varchar(32) NOT NULL,
  `gamesWon` tinyint(3) unsigned default NULL,
  `gamesLost` tinyint(3) unsigned default NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

