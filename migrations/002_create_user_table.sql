CREATE TABLE user
(
    id                INTEGER PRIMARY KEY,
    username          TEXT NOT NULL UNIQUE,
    password          TEXT NOT NULL,
    force_pass_change INTEGER NOT NULL DEFAULT 1, -- 0 no, 1 force password change
    permissions       INTEGER NOT NULL DEFAULT 0 -- 0 normal user, 1 admin
);

CREATE TABLE session_key
(
    id           INTEGER PRIMARY KEY,
    user_id      INTEGER NOT NULL,
    ip           TEXT NOT NULL,
    browser_info TEXT NOT NULL,
    key_hash     TEXT NOT NULL UNIQUE,
    created_at   DATETIME DEFAULT CURRENT_TIMESTAMP,
    expires_at   DATETIME,
    last_used    DATETIME,

    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
);