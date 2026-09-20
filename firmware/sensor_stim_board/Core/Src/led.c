#include "user_comm.h"

float max_cap_val_led = 0;

static const led_gpio_t led_blue[] =
    {
        {LED_B_0_GPIO_Port, LED_B_0_Pin},
        {LED_B_1_GPIO_Port, LED_B_1_Pin},
        {LED_B_2_GPIO_Port, LED_B_2_Pin},
        {LED_B_3_GPIO_Port, LED_B_3_Pin},
        {LED_B_4_GPIO_Port, LED_B_4_Pin},
        {LED_B_5_GPIO_Port, LED_B_5_Pin},
        {LED_B_6_GPIO_Port, LED_B_6_Pin},
        {LED_B_7_GPIO_Port, LED_B_7_Pin},
        {LED_B_8_GPIO_Port, LED_B_8_Pin},
        {LED_B_9_GPIO_Port, LED_B_9_Pin},
};

static const led_gpio_t led_red[] =
    {
        {LED_R_0_GPIO_Port, LED_R_0_Pin},
        {LED_R_1_GPIO_Port, LED_R_1_Pin},
        {LED_R_2_GPIO_Port, LED_R_2_Pin},
        {LED_R_3_GPIO_Port, LED_R_3_Pin},
        {LED_R_4_GPIO_Port, LED_R_4_Pin},
        {LED_R_5_GPIO_Port, LED_R_5_Pin},
        {LED_R_6_GPIO_Port, LED_R_6_Pin},
        {LED_R_7_GPIO_Port, LED_R_7_Pin},
        {LED_R_8_GPIO_Port, LED_R_8_Pin},
        {LED_R_9_GPIO_Port, LED_R_9_Pin},
};

static void led_all_off(const led_gpio_t *leds, uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_SET);
    }
}

void all_blue_off(void)
{
    led_all_off(led_blue, sizeof(led_blue) / sizeof(led_blue[0]));
}

void all_red_off(void)
{
    led_all_off(led_red, sizeof(led_red) / sizeof(led_red[0]));
}

void light_base_count(uint8_t led_count, const led_gpio_t *leds, uint8_t total_leds)
{
    led_all_off(leds, total_leds);
    if (led_count > TOTAL_LED)
        led_count = TOTAL_LED;

    if (led_count == 0)
        return;
    for (uint8_t i = 0; i < led_count; i++)
    {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_RESET);
    }
}

void light_blue_led(uint8_t led_count)
{
    light_base_count(led_count, led_blue, sizeof(led_blue) / sizeof(led_blue[0]));
}

void light_red_led(uint8_t led_count)
{
    light_base_count(led_count, led_red, sizeof(led_red) / sizeof(led_red[0]));
}

void set_sensitive_led(void)
{
    static uint8_t last_sensitive = 0;
    if (sensitive == last_sensitive)
        return;

    DBG_PRINTF("Setting LED for sensitive: %d\r\n", sensitive);

    // 根据sensitive设置led的数量
    light_blue_led(sensitive);
    last_sensitive = sensitive;
}

void sens_led_task(void)
{
    static uint32_t last_run_ts = 0;

    if (HAL_GetTick() - last_run_ts < 200)
        return;

    set_sensitive_led();

    last_run_ts = HAL_GetTick();
}

void working_led_task(void)
{
    static uint32_t last_run_ts = 0;
    if (HAL_GetTick() - last_run_ts < 200)
        return;

    HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);

    last_run_ts = HAL_GetTick();
}

void max_cap_led_task(void)
{
    static uint32_t last_run_ts = 0;
    if (HAL_GetTick() - last_run_ts < 50)
        return;

    if (max_cap_val_led > MAX_CAP)
        max_cap_val_led = MAX_CAP;

    uint8_t led_count = (uint8_t)(max_cap_val_led / CAP_LED_STEP);

    light_red_led(led_count);

    last_run_ts = HAL_GetTick();
}

void LED_Test(void)
{
    for(int i = 0; i < 10; i++)
    {
        HAL_GPIO_WritePin(led_red[i].port, led_red[i].pin, GPIO_PIN_SET);
        HAL_IWDG_Refresh(&hiwdg1);
        HAL_Delay(200);
        HAL_GPIO_WritePin(led_red[i].port, led_red[i].pin, GPIO_PIN_RESET);
    }
}
