# Unnoticeable
### Flag: FLAG{w4tch_car3fully_f0r_nu1l_byte5}
#### Description:
Менеджер заметок... Ну что здесь может пойти не так? (в этой задаче необходимо получить удаленное исполнение кода)
#### Комментарий (для жюри):
Таска на off-by-null. Зануляем поле deleted следующего чанка массива -> UAF -> лик libc из unsorted BK -> Double Free -> nearly arbitrary read/write -> переписать malloc_hook/free_hook на one_gadget / binary_gadget -> получить flag  
