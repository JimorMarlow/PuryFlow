// InitFilter.h
// Шаблон для инициализации первым элементом фильтров GyverFilters,
// чтобы можно было сразу получать первое усредненное значение без уходов в ноль

// Ориентирован на GyverFilters - библиотека с некоторыми удобными фильтрами для Arduino
// Документация: https://alexgyver.ru/gyverfilters/
// GitHub: https://github.com/GyverLibs/GyverFilters

#ifndef _InitFilter_h
#define _InitFilter_h

template <typename FILTER, typename TYPE, int first_count >
class InitFilter {
public:
    TYPE filtered(TYPE value) {	// возвращает фильтрованное значение
        if(!is_init_)
        {
            // первоначальная накачка фильтра первым значением
            for(int i = 0 ; i < first_count; ++i)     
            {
                filter_.filtered(value);
            }
            is_init_ = true;
        }
        return filter_.filtered(value);
    }

    FILTER& filter() { 
        return filter_; 
    }
    
private:
    FILTER filter_;
    bool is_init_ = false;
};
#endif//_InitFilter_h

