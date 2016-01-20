PRAGMA user_version = 30; /* Tizen 3.0 */
PRAGMA journal_mode = PERSIST;
PRAGMA foreign_keys = ON;

BEGIN EXCLUSIVE TRANSACTION;

CREATE TABLE widget_class (
  classid       TEXT NOT NULL,
  update_period INTEGER DEFAULT 0,
  setup_appid   TEXT,
  appid         TEXT NOT NULL,
  pkgid         TEXT NOT NULL,
  PRIMARY KEY(classid)
);

CREATE TABLE support_size (
  classid TEXT NOT NULL,
  preview TEXT NOT NULL,
  frame   INTEGER DEFAULT 0,
  width   INTEGER NOT NULL,
  height  INTEGER NOT NULL,
  FOREIGN KEY(classid) REFERENCES widget_class (classid) ON DELETE CASCADE
);

CREATE TABLE label (
  classid TEXT NOT NULL,
  locale  TEXT NOT NULL,
  label   TEXT NOT NULL,
  FOREIGN KEY(classid) REFERENCES widget_class (classid) ON DELETE CASCADE
);

CREATE TABLE icon (
  classid TEXT NOT NULL,
  locale  TEXT NOT NULL,
  icon    TEXT NOT NULL,
  FOREIGN KEY(classid) REFERENCES widget_class (classid) ON DELETE CASCADE
);

COMMIT TRANSACTION;
