#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xd1b09e08, "module_layout" },
	{ 0x3474dcb7, "usb_deregister" },
	{ 0x2ec94476, "usb_register_driver" },
	{ 0xc98b0489, "of_led_classdev_register" },
	{ 0x44b5ee9a, "kasprintf" },
	{ 0x49e6fd3f, "dev_driver_string" },
	{ 0xd29ba552, "_dev_info" },
	{ 0xeea2966b, "usb_get_intf" },
	{ 0x1d627438, "usb_get_dev" },
	{ 0xc3c01ffa, "kmem_cache_alloc_trace" },
	{ 0xe1ea0878, "kmalloc_caches" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xe4643922, "usb_control_msg" },
	{ 0x7c32d0f0, "printk" },
	{ 0x37a0cba, "kfree" },
	{ 0xbbbd175f, "usb_put_dev" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0x92083a0b, "led_classdev_unregister" },
	{ 0xbdfb6dbb, "__fentry__" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=usbcore";

MODULE_ALIAS("usb:vCAFEp4013d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "59D3F16DA158363201260EF");
