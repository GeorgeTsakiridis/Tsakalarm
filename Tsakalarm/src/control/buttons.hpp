#ifndef CONTROL_BUTTONS_HPP
#define CONTROL_BUTTONS_HPP

void buttons_init();
void buttons_set_callbacks(void(*back_callback_func)(), void(*top_callback_func)());

void buttons_update();

#endif // CONTROL_BUTTONS_HPP