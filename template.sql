PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE "client" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "location_id" INTEGER NOT NULL,
    "name" TEXT,
    "surname" TEXT,
    "email" TEXT,
    "tel" TEXT,
    "cel" TEXT,
    "dni" TEXT,
    "address" TEXT,
    "people_num" INTEGER,
    "tent_num" INTEGER,
    "in_time" INTEGER,
    "out_time" INTEGER,
    "create_time" INTEGER,
    "update_time" INTEGER,
    FOREIGN KEY (location_id) REFERENCES location(id)
);
CREATE TABLE "location" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "name" TEXT NOT NULL,
    "type" INTEGER DEFAULT (0),
    "xpos" INTEGER DEFAULT (0),
    "ypos" INTEGER DEFAULT (0),
    "create_time" INTEGER,
    "update_time" INTEGER
);
CREATE TABLE "vehicle" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "client_id" INTEGER NOT NULL,
    "model" TEXT,
    "patent" TEXT,
    "size" TEXT,
    "create_time" INTEGER,
    "update_time" INTEGER,
    FOREIGN KEY (client_id) REFERENCES client(id)
);
CREATE TABLE "config" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "key" TEXT NOT NULL,
    "value" TEXT NOT NULL
);
INSERT INTO config ("id", "key", "value") values ('1', 'db_version', '1');
COMMIT;
