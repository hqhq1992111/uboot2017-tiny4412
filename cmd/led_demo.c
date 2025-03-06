#include <common.h>
#include <dm-demo.h>
#include <mapmem.h>
#include <asm/io.h>

static int do_led_demo_set_status(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
    int ret = -1;
	struct udevice *dev;

	ret = led_get_by_label("led0 label", &dev);
    if (ret) {
        printf("led_get_by_label failed!\n");
        return ret;
    }
    if (strcmp("on", argv[0]) == 0) {
        printf("set led on\n");
        led_set_on(dev, 1);
    } else if (strcmp("off", argv[0]) == 0) {
        printf("set led off\n");
        led_set_on(dev, 0);
    }
	return ret;
}

int do_led_demo_probe(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct udevice *dev;
	int i, ret;

	printf("do_led_demo_probe!\n");

	for (i = 0, ret = uclass_first_device(UCLASS_LED, &dev);
	     dev;
	     ret = uclass_next_device(&dev)) {
        printf("%s come probed!\n", dev->name);
	}

	return cmd_process_error(cmdtp, ret);
}

static cmd_tbl_t led_demo_commands[] = {
	U_BOOT_CMD_MKENT(probe, 0, 1, do_led_demo_probe, "", ""),
	U_BOOT_CMD_MKENT(status, 1, 1, do_led_demo_set_status, "", ""),
};

static int do_led_demo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *demo_cmd;
	int devnum = 0;
	int ret;

	if (argc < 2)
		return CMD_RET_USAGE;
	demo_cmd = find_cmd_tbl(argv[1], led_demo_commands,
				ARRAY_SIZE(led_demo_commands));
	argc -= 2;
	argv += 2;

	if ((!demo_cmd || argc > demo_cmd->maxargs) ||
	    ((demo_cmd->name[0] != 'p') && (argc < 1)))
		return CMD_RET_USAGE;

	ret = demo_cmd->cmd(demo_cmd, flag, argc, argv);

	return cmd_process_error(demo_cmd, ret);
}

U_BOOT_CMD(
	led_demo,   3,      1,      do_led_demo,
	"Driver model (dm) led demo operations",
	"probe                     probe available LED devices\n"
	"status [on|off]          set LED on|off state"
);

