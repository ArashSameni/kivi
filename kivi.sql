create database kivi_db;

CREATE TABLE Users (
    Id int NOT NULL AUTO_INCREMENT,
    Type int NOT NULL,
    FirstName varchar(100) NOT NULL,
    LastName varchar(100) NOT NULL,
    NationalCode varchar(10) NOT NULL,
    Username varchar(50) NOT NULL,
    Password varchar(50) NOT NULL,
    Email varchar(100) NOT NULL,
    CoronaCode int NOT NULL,
    PRIMARY KEY (ID)
);

CREATE TABLE NormalInfo (
    UserId int NOT NULL,
    BloodGroup varchar(3),
    Age int,
    Height int,
    Weight int,
    PRIMARY KEY (UserId)
);

CREATE TABLE BusInfo (
    UserId int NOT NULL,
    PlaceName varchar(100) NOT NULL,
    CityName varchar(100) NOT NULL,
    AreaId int NOT NULL,
    PRIMARY KEY (UserId)
);

CREATE TABLE AreaVisits (
    UserId int NOT NULL,
    AreaId int NOT NULL,
    Stamp int NOT NULL
);

CREATE TABLE UserVisits (
    UserId1 int NOT NULL,
    UserId2 int NOT NULL,
    Stamp int NOT NULL
);

CREATE TABLE CoronaTests (
    UserId int NOT NULL,
    CoronaCode int NOT NULL,
    Stamp int NOT NULL
);

INSERT INTO Users (Type, FirstName, LastName, NationalCode, Username, Password, Email, CoronaCode)
VALUES (3, 'admin', 'admin', '0', 'admin', 'firns678', 'admin', 0);
//firns678 = admin123