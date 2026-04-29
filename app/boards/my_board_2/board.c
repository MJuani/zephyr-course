#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int board_my_board_2_init( void ){
    printf("Board Initialized\r\n" );

    return 0;
}

SYS_INIT(board_my_board_2_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
