#ifndef BASE_ENTRY_POINT_H
#define BASE_ENTRY_POINT_H

internal void main_thread_entry(int argc, char **argv);
internal void worker_thread_entry(void (*entry_point)(void *params), void *params);

#endif /* BASE_ENTRY_POINT_H */
