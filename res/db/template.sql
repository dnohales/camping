PRAGMA foreign_keys=OFF;
CREATE TABLE "client" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "name" TEXT,
    "surname" TEXT,
    "email" TEXT,
    "tel" TEXT,
    "dni" TEXT,
    "address" TEXT,
    "create_time" TEXT,
    "update_time" TEXT
);
CREATE TABLE "reservation" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "client_id" INTEGER NOT NULL,
    "location_id" INTEGER NOT NULL,
    "people_num" INTEGER,
    "tent_num" INTEGER,
    "in_time" TEXT,
    "out_time" TEXT,
    "beck" TEXT,
    "create_time" TEXT,
    "update_time" TEXT,
    FOREIGN KEY (client_id) REFERENCES client(id),
    FOREIGN KEY (location_id) REFERENCES location(id)
);
CREATE TABLE "location" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "name" TEXT NOT NULL,
    "type" INTEGER DEFAULT (0),
    "xpos" INTEGER DEFAULT (0),
    "ypos" INTEGER DEFAULT (0),
    "create_time" TEXT,
    "update_time" TEXT
);
CREATE TABLE "vehicle" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "reservation_id" INTEGER NOT NULL,
    "model" TEXT,
    "patent" TEXT,
    "size" TEXT,
    "create_time" TEXT,
    "update_time" TEXT,
    FOREIGN KEY (reservation_id) REFERENCES reservation(id)
);
CREATE TABLE "config" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "key" TEXT NOT NULL,
    "value" TEXT NOT NULL
);
INSERT INTO config ("id", "key", "value") values ('1', 'db_version', '2');
PRAGMA foreign_keys=ON;
