-- MySQL dump 8.23
--
-- Host: localhost    Database: greybox
---------------------------------------------------------
-- Server version	3.23.58

--
-- Table structure for table `klub`
--

CREATE TABLE klub (
  klub_ID int(10) unsigned NOT NULL auto_increment,
  misto varchar(255) default NULL,
  nazev varchar(255) NOT NULL default '',
  kratky_nazev varchar(32) NOT NULL default '',
  komentar blob,
  PRIMARY KEY  (klub_ID)
) TYPE=InnoDB;

--
-- Table structure for table `tym`
--

CREATE TABLE tym (
  tym_ID int(10) unsigned NOT NULL auto_increment,
  klub_ID int(10) unsigned NOT NULL default '0',
  nazev varchar(255) NOT NULL default '',
  komentar blob,
  PRIMARY KEY  (tym_ID),
  KEY klub_ID (klub_ID),
  constraint fk_tym_klub foreign key (klub_ID) references klub (klub_ID) on delete restrict
) TYPE=InnoDB;


--
-- Table structure for table `clovek`
--

CREATE TABLE clovek (
  clovek_ID int(10) unsigned NOT NULL auto_increment,
  jmeno varchar(40) NOT NULL default '',
  nick varchar(255) default NULL,
  prijmeni varchar(60) NOT NULL default '',
  narozen date default NULL,
  klub_ID int(10) unsigned default NULL,
  debater tinyint(1) NOT NULL default '0',
  komentar blob,
  prava_lidi tinyint(3) unsigned NOT NULL default '0',
  prava_kluby tinyint(3) unsigned NOT NULL default '0',
  prava_souteze tinyint(3) unsigned NOT NULL default '0',
  prava_debaty tinyint(3) unsigned NOT NULL default '0',
  PRIMARY KEY  (clovek_ID),
  KEY klub_ID (klub_ID),
  KEY prijmeni (prijmeni,jmeno,nick),
  constraint fk_clovek_klub foreign key (klub_ID) references klub (klub_ID) on delete restrict
) TYPE=InnoDB;

--
-- Table structure for table `soutez`
--

CREATE TABLE soutez (
  soutez_ID int(10) unsigned NOT NULL auto_increment,
  rocnik tinyint(3) unsigned NOT NULL default '0',
  jazyk enum('cz','en','de','fr') default NULL,
  nazev varchar(255) NOT NULL default '',
  komentar blob,
  zamceno tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (soutez_ID)
) TYPE=InnoDB;

--
-- Table structure for table `turnaj`
--

CREATE TABLE turnaj (
  turnaj_ID int(10) unsigned NOT NULL auto_increment,
  soutez_ID int(10) unsigned NOT NULL default '0',
  nazev varchar(255) NOT NULL default '',
  datum_od date NOT NULL default '0000-00-00',
  datum_do date NOT NULL default '0000-00-00',
  komentar blob,
  PRIMARY KEY  (turnaj_ID),
  KEY soutez_ID (soutez_ID),
  KEY datum_od (datum_od),
  constraint fk_turnaj_soutez foreign key (soutez_ID) references soutez (soutez_ID) on delete restrict
) TYPE=InnoDB;

--
-- Table structure for table `debata`
--

CREATE TABLE debata (
  debata_ID int(10) unsigned NOT NULL auto_increment,
  soutez_ID int(10) unsigned NOT NULL default '0',
  turnaj_ID int(10) unsigned default NULL,
  teze_ID int(10) unsigned NOT NULL default '0',
  datum date NOT NULL default '0000-00-00',
  misto varchar(255) default NULL,
  komentar blob,
  vitez tinyint(1) NOT NULL default '0',
  presvedcive tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (debata_ID),
  KEY soutez_ID (soutez_ID),
  KEY turnaj_ID (turnaj_ID),
  constraint fk_debata_soutez foreign key (soutez_ID) references soutez (soutez_ID) on delete restrict,
  constraint fk_debata_turnaj foreign key (turnaj_ID) references turnaj (turnaj_ID) on delete restrict
) TYPE=InnoDB;


--
-- Table structure for table `clovek_debata`
--

CREATE TABLE clovek_debata (
  dc_ID int(10) unsigned NOT NULL auto_increment,
  debata_ID int(10) unsigned NOT NULL default '0',
  clovek_ID int(10) unsigned NOT NULL default '0',
  role enum('r','o','a1','a2','a3','n1','n2','n3') NOT NULL default 'r',
  kidy tinyint(3) unsigned default NULL,
  rozhodnuti tinyint(1) default NULL,
  presvedcive tinyint(1) default NULL,
  PRIMARY KEY  (dc_ID),
  KEY debata_ID (debata_ID),
  KEY clovek_ID (clovek_ID),
  constraint fk_cd_debata foreign key (debata_ID) references debata (debata_ID) on delete cascade,
  constraint fk_cd_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete restrict
) TYPE=InnoDB;

--
-- Table structure for table `clovek_debata_ibody`
--

CREATE TABLE clovek_debata_ibody (
  cdi_ID int(10) unsigned NOT NULL auto_increment,
  clovek_ID int(10) unsigned NOT NULL default '0',
  debata_ID int(10) unsigned NOT NULL default '0',
  rocnik tinyint(3) unsigned NOT NULL default '0',
  role enum('debater','rozhodci','trener','organizator') NOT NULL default 'debater',
  ibody decimal(5,3) NOT NULL default '0.000',
  PRIMARY KEY  (cdi_ID),
  KEY clovek_ID (clovek_ID),
  KEY debata_ID (debata_ID),
  KEY role (role),
  constraint fk_cdi_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete cascade,
  constraint fk_cdi_debata foreign key (debata_ID) references debata (debata_ID) on delete cascade
) TYPE=InnoDB;

--
-- Table structure for table `clovek_klub`
--

CREATE TABLE clovek_klub (
  ck_ID int(10) unsigned NOT NULL auto_increment,
  clovek_ID int(10) unsigned NOT NULL default '0',
  klub_ID int(10) unsigned NOT NULL default '0',
  rocnik tinyint(3) unsigned NOT NULL default '0',
  role enum('t') NOT NULL default 't',
  PRIMARY KEY  (ck_ID),
  KEY clovek_ID (clovek_ID),
  KEY klub_ID (klub_ID),
  constraint fk_ck_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete restrict,
  constraint fk_ck_klub foreign key (klub_ID) references klub (klub_ID) on delete cascade
) TYPE=InnoDB;

--
-- Table structure for table `clovek_turnaj`
--

CREATE TABLE clovek_turnaj (
  ct_ID int(10) unsigned NOT NULL auto_increment,
  clovek_ID int(10) unsigned NOT NULL default '0',
  turnaj_ID int(10) unsigned NOT NULL default '0',
  role enum('o') NOT NULL default 'o',
  PRIMARY KEY  (ct_ID),
  KEY clovek_ID (clovek_ID),
  KEY turnaj_ID (turnaj_ID),
  constraint fk_ctu_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete restrict,
  constraint fk_ctu_turnaj foreign key (turnaj_ID) references turnaj (turnaj_ID) on delete cascade
) TYPE=InnoDB;

--
-- Table structure for table `clovek_tym`
--

CREATE TABLE clovek_tym (
  ct_ID int(10) unsigned NOT NULL auto_increment,
  clovek_ID int(10) unsigned NOT NULL default '0',
  tym_ID int(10) unsigned NOT NULL default '0',
  aktivni tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (ct_ID),
  KEY clovek_ID (clovek_ID),
  KEY tym_ID (tym_ID),
  constraint fk_cty_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete cascade,
  constraint fk_cty_tym foreign key (tym_ID) references tym (tym_ID) on delete cascade
) TYPE=InnoDB;


--
-- Table structure for table `debata_tym`
--

CREATE TABLE debata_tym (
  dt_ID int(10) unsigned NOT NULL auto_increment,
  debata_ID int(10) unsigned NOT NULL default '0',
  tym_ID int(10) unsigned NOT NULL default '0',
  pozice tinyint(1) NOT NULL default '0',
  body tinyint(3) unsigned default NULL,
  PRIMARY KEY  (dt_ID),
  KEY debata_ID (debata_ID),
  KEY tym_ID (tym_ID),
  constraint fk_dt_debata foreign key (debata_ID) references debata (debata_ID) on delete cascade,
  constraint fk_dt_tym foreign key (tym_ID) references tym (tym_ID) on delete restrict
) TYPE=InnoDB;

--
-- Table structure for table `kontakt`
--

CREATE TABLE kontakt (
  kontakt_ID int(10) unsigned NOT NULL auto_increment,
  clovek_ID int(10) unsigned NOT NULL default '0',
  druh enum('telefon','email','adresa','icq','jabber','web') NOT NULL default 'telefon',
  tx varchar(255) NOT NULL default '',
  viditelnost tinyint(3) unsigned NOT NULL default '1',
  PRIMARY KEY  (kontakt_ID),
  KEY clovek_ID (clovek_ID),
  constraint fk_kontakt_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete cascade
) TYPE=InnoDB;

--
-- Table structure for table `login`
--

CREATE TABLE login (
  login_ID int(10) unsigned NOT NULL auto_increment,
  clovek_ID int(10) unsigned NOT NULL default '0',
  username varchar(32) NOT NULL default '',
  password varchar(35) NOT NULL default '',
  PRIMARY KEY  (login_ID),
  KEY clovek_ID (clovek_ID),
  KEY username (username),
  constraint fk_login_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete cascade
) TYPE=InnoDB;

--
-- Table structure for table `rozhodci`
--

CREATE TABLE rozhodci (
  rozhodci_ID int(10) unsigned NOT NULL auto_increment,
  clovek_ID int(10) unsigned NOT NULL default '0',
  misto varchar(255) default NULL,
  jazyk set('cz','en','de','fr') default NULL,
  rocnik tinyint(3) unsigned NOT NULL default '0',
  PRIMARY KEY  (rozhodci_ID),
  KEY clovek_ID (clovek_ID),
  constraint fk_rozhodci_clovek foreign key (clovek_ID) references clovek (clovek_ID) on delete cascade
) TYPE=InnoDB;

--
-- Table structure for table `teze`
--

CREATE TABLE teze (
  teze_ID int(10) unsigned NOT NULL auto_increment,
  jazyk enum('cz','en','de','fr') NOT NULL default 'cz',
  tx varchar(255) NOT NULL default '',
  komentar blob,
  PRIMARY KEY  (teze_ID),
  KEY jazyk (jazyk)
) TYPE=InnoDB;

--
-- Table structure for table `soutez_teze`
--

CREATE TABLE soutez_teze (
  st_ID int(10) unsigned NOT NULL auto_increment,
  soutez_ID int(10) unsigned NOT NULL default '0',
  teze_ID int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (st_ID),
  KEY soutez_ID (soutez_ID),
  KEY teze_ID (teze_ID),
  constraint fk_st_soutez foreign key (soutez_ID) references soutez (soutez_ID) on delete cascade,
  constraint fk_st_teze foreign key (teze_ID) references teze (teze_ID) on delete restrict
) TYPE=InnoDB;
