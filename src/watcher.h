struct watcher {
    unsigned offset;
    void(*on_rise)(void);
    void(*on_fall)(void);
};
int watch_lines(struct watcher* watchers, unsigned watcher_n);

