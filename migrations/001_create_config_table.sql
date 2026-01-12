CREATE TABLE app_setting
(
    name TEXT    NOT NULL UNIQUE,
    data TEXT    NOT NULL,
    PRIMARY KEY (name)
);

-- Default settings
INSERT INTO app_setting (name, data) VALUES
('default_expiration_hours',   '24'), -- Default expiration (in hours) applied to new share links if admin doesn’t override
('max_expiration_days',        '30'), -- Maximum expiration (in days) any share link may have
('enable_one_time_links',      'true'), -- Whether admins can create single‑use download links
('cleanup_interval_minutes',   '60'), -- How often (in minutes) the background job runs to remove expired files/links
('admin_session_timeout_secs', '1800'), -- Inactivity timeout for admin login (in seconds)
('password_policy',            '{ "min_len": 8, "require_lower": true, "require_digit_upper": true, "require_digit": true }'), -- JSON‑encoded rules for share‑link password strength
('log_retention_days',         '7'), -- Number of days to keep audit logs (admin actions, uploads, downloads)
('maintenance_mode',           'false'), -- Toggle for putting the site into maintenance (blocking uploads/downloads except to admin)
('session_expiration_days',    '30'); -- Session expiration time in days