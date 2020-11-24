#!/bin/bash

RED='\e[1;41m'     # 红
GREEN='\e[1;42m'   # 绿
YELLOW='\e[43;34m' # 黄
BLUE='\e[1;44m'    # 蓝
PINK='\e[1;45m'    # 粉红
RES='\e[0m'        # 清除颜色

zodiac() {
    animal=("猪" "鼠" "牛" "虎" "兔" "龙" "蛇" "马" "羊" "猴" "鸡" "狗")
    delta=$(expr $year - 2019)
    echo -e "${GREEN}zodiac: ${animal[$(expr ${delta} % 12)]}${RES}"
    return
}

constellation() {
    [[ $month = 1 && $day > 19 || $month = 2 && $day < 19 ]] && constell='水瓶座'
    [[ $month = 2 && $day > 20 || $month = 3 && $day < 21 ]] && constell='双鱼座'
    [[ $month = 3 && $day > 20 || $month = 4 && $day < 20 ]] && constell='白羊座'
    [[ $month = 4 && $day > 19 || $month = 5 && $day < 21 ]] && constell='金牛座'
    [[ $month = 5 && $day > 20 || $month = 6 && $day < 22 ]] && constell='双子座'
    [[ $month = 6 && $day > 21 || $month = 7 && $day < 23 ]] && constell='巨蟹座'
    [[ $month = 7 && $day > 22 || $month = 8 && $day < 23 ]] && constell='狮子座'
    [[ $month = 8 && $day > 22 || $month = 9 && $day < 23 ]] && constell='处女座'
    [[ $month = 9 && $day > 22 || $month = 10 && $day < 24 ]] && constell='天秤座'
    [[ $month = 10 && $day > 23 || $month = 11 && $day < 22 ]] && constell='天蝎座'
    [[ $month = 11 && $day > 22 || $month = 12 && $day < 20 ]] && constell='射手座'
    [[ $month = 12 && $day > 21 || $month = 2 && $day < 19 ]] && constell='摩羯座'
    echo -e "${BLUE}constellation: $constell${RES}"
    return
}

echo -e "Hello! I'm Fortune Master!"
while :; do
    read -p "Please tell me a birthday (yyyy/MM/dd): " birthday
    if [ -z ${birthday} ]; then
        continue
    fi
    array=(${birthday//// })
    # echo "$array"
    cnt=${#array[*]}
    # echo "input number=${cnt}"
    if [ ${cnt} -ne 3 ]; then
        echo -e "${RED}input count invalid${RES}"
        continue
    fi
    # reg="\d{4}/\d{1,2}/\d{1,2}"
    # echo ${birthday} | grep -E --color ${reg}
    year=${array[0]}
    month=$(expr ${array[1]} + 0)
    day=$(expr ${array[2]} + 0)
    if [[ ${month} -lt 1 || ${month} -gt 12 ]]; then
        echo -e "${RED}invalid month${RES}"
        continue
    fi
    if [[ ${day} -lt 1 || ${day} -gt 31 ]]; then
        echo -e "${RED}invalid day${RES}"
        continue
    fi
    echo -e "Your birthday is ${YELLOW}${year}年${month}月${day}日${RES}"
    zodiac
    constellation
done
