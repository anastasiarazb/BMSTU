-- Прочитать файл с записями со всеми кавычками
airport_file_strings = LOAD 'L_AIRPORT_ID.csv' using PigStorage(',') AS (
        code:chararray,
        name_part1:chararray,
        name_part2:chararray);
 
airport_records = FILTER airport_file_strings BY code != 'Code';
 
airport_key_value = FOREACH airport_records GENERATE
        (INT)REPLACE(code, '\\"', '')  as code,
        CONCAT(name_part1, name_part2) as name;
 
flights_file_strings = LOAD 'T_ONTIME.csv' using PigStorage(',') AS (
        year:int,
        quarter:int,
        month:int,
        day_of_month:int,
        day_of_week:int,
        fl_date:chararray,
        unique_carrier:chararray,
        airline_id:int,
        carrier:chararray,
        tail_num:chararray,
        fl_num:chararray,
        origin_airport_id:int,
        origin_airport_seq_id:int,
        origin_city_market_id:int,
        dest_airport_id:int,
        wheels_on:chararray,
        arr_time:chararray,
        arr_delay:float,
        arr_delay_new:float,
        cancelled:float,
        cancellation_code:chararray,
        air_time:float,
        distance:float);

flights_delayed = FILTER flights_file_strings BY (chararray)year != 'YEAR'
                                                AND cancelled == 0
                                                AND (dest_airport_id is not null)
                                                AND (arr_delay_new   is not null) AND (arr_delay_new > 0);
 
flight_key_value = FOREACH flights_delayed GENERATE
                   (INT)dest_airport_id,
                 (FLOAT)arr_delay_new;
 
 
 
 
 
-- генерация ВИРТУАЛЬНОЙ таблицы (код аэропорта, задержка, код аэропорта, имя)
joined_key_values = JOIN flight_key_value by dest_airport_id,
                        airport_key_value by code;
             
-- генерация таблицы (имя, задержка)
joined_brief = FOREACH joined_key_values GENERATE
               name,
               arr_delay_new as delay;
 
-- группировка по имени для агрегирующих функций MIN,AVG,MAX
grouped_by_name = GROUP joined_brief BY name;
 
final = FOREACH grouped_by_name GENERATE
        group as name,
        MIN(joined_brief.delay) as delay_min,
        MAX(joined_brief.delay) as delay_max,
        AVG(joined_brief.delay) as delay_average;
        
-- Доп. задание Для каждого аэропорта прибытия вычислить день недели, в который произошло больше всего отмен

flights_cancelled = FILTER flights_file_strings BY (chararray)year != 'YEAR'
                                                AND cancelled == 1.00
                                                AND dest_airport_id is not null;
                                                
flight_key_value = FOREACH flights_cancelled GENERATE
                   (INT)dest_airport_id,
                   (INT)day_of_week;

-- генерация ВИРТУАЛЬНОЙ таблицы (код аэропорта, день недели, код аэропорта, имя)
joined_key_values = JOIN flight_key_value by dest_airport_id,
                        airport_key_value by code;
                        
-- редуцирование до (код аэропорта, имя, день недели)           
id_name_day_of_week = FOREACH joined_key_values GENERATE
                            flight_key_value::dest_airport_id as dest_airport_id,
                            airport_key_value::name           as name,
                            flight_key_value::day_of_week     as day_of_week;
                            
-- Группировка (имя, день недели)
grouped_by_name = GROUP id_name_day_of_week BY (name, day_of_week);

-- генерация таблицы, развернутой обратно, и при этом подсчет, сколько для каждой пары (имя, день недели) было задержек
cancel_table = FOREACH grouped_by_name GENERATE
                                 FLATTEN(group) as (name, day_of_week),
                    (INT)COUNT(id_name_day_of_week) as count_cancels;
                    
-- Группировка по имени
cancel_group_table = GROUP cancel_table BY name;

-- Формирование таблицы с максимальным кол-вом отмен по дню недели
max_cancels_table = FOREACH cancel_group_table GENERATE
                            group as name,
                            MAX(cancel_table.count_cancels) as max_count_cancels;
                            
-- Объединить выборку максимумов и исходную таблицу
result_table = JOIN cancel_table BY name,
               max_cancels_table BY name;
               
result_table = FOREACH result_table GENERATE cancel_table::name as airport_name,
                                      cancel_table::day_of_week as day_of_week,
                                    cancel_table::count_cancels as count_cancels,
                               max_cancels_table::max_count_cancels as max_cancels;
                               
result_table = FILTER result_table BY (count_cancels == max_cancels);

result_table = FOREACH result_table GENERATE
                        airport_name,
                        day_of_week,
                        max_cancels;
                        
-- Заменить номера дней на их названия из соответствующей таблицы                   
days = LOAD 'DAYS_OF_WEEK.csv' using PigStorage(',') AS (
        number:int,
        day_name:chararray);
        
result_table = JOIN days BY number,
            result_table BY day_of_week;
            
result_table = FOREACH result_table GENERATE
                    airport_name,
                    day_name,
                    max_cancels;
                                        
--STORE final INTO 'pig_output' using PigStorage(',');
--dump final;
STORE result_table INTO 'pig_output' using PigStorage(',');
dump result_table;
quit;

/*
Processing the results

To work on the results of the group operator, you will want to use a FOREACH. This is a simple loop construct that works on a relation one row at a time. You can apply it to any relation, but it’s most frequently used on results of grouping, as it allows you to apply aggregation functions to the collected bags.

Referring to somebag.some_field in a FOREACH operator essentially means “for each tuple in the bag, give me some_field in that tuple”. So you can do things like
age_counts = FOREACH by_age GENERATE
  group as age,  -- the key you grouped on
 COUNT(my_data), -- the number of people with this age
 MAX(my_data.height); -- the maximum height of people with this age

Note that all the functions in this example are aggregates. That’s because they are things we can do to a collection of values. Folks sometimes try to apply single-item operations in a foreach — like transforming strings or checking for specific values of a field. Remember, my_data.height doesn’t give you a single height element — it gives you all the heights of all people in a given age group.*/
