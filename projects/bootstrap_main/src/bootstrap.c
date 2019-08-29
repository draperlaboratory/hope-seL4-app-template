/* Include config variables. */
#include <autoconf.h>

/* Basics */
#include <stdio.h>
#include <assert.h>

#include <sel4/sel4.h>

#include <simple/simple.h>
#include <simple-default/simple-default.h>

#include <allocman/allocman.h>
#include <allocman/bootstrap.h>
#include <allocman/vka.h>

#include <vka/object.h>
#include <vka/capops.h>

//#include <sel4platsupport/device.h>
#include <sel4platsupport/platsupport.h>
//#include <sel4platsupport/serial.h>
//#include <sel4platsupport/bootinfo.h>
#include <sel4platsupport/io.h>

#include <sel4utils/vspace.h>
//#include <sel4utils/stack.h>
//#include <sel4utils/process.h>
//#include <sel4test/test.h>

#include <platsupport/chardev.h>
#include <platsupport/serial.h>
#include <platsupport/plat/serial.h>

#include <utils/util.h>

//#include <vspace/vspace.h>

/* global environment variables */

/* seL4_BootInfo defined in bootinfo.h */
seL4_BootInfo *info;

/* simple_t defined in simple.h */
simple_t simple;

/* vka_t defined in vka.h */
vka_t vka;

/* allocman_t defined in allocman.h */
allocman_t *allocman;

/* dimensions of virtual memory for the allocator to use */
#define ALLOCATOR_VIRTUAL_POOL_SIZE ((1 << seL4_PageBits) * 100)

/* static memory for the allocator to bootstrap with */
#define ALLOCATOR_STATIC_POOL_SIZE ((1 << seL4_PageBits) * 20)

static char allocator_mem_pool[ALLOCATOR_STATIC_POOL_SIZE];

/* static memory for virtual memory bootstrapping */
static sel4utils_alloc_data_t data;

int main(void)
{
	// from dynamic-1
	int error;
	ps_io_ops_t io_ops;

	info = platsupport_get_bootinfo();
	if (info == NULL) {
		ZF_LOGF("Failed to get bootinfo\n");
	}

	simple_default_init_bootinfo(&simple, info);

	allocman = bootstrap_use_current_simple(&simple, ALLOCATOR_STATIC_POOL_SIZE, &allocator_mem_pool);
	if (allocman == NULL) {
		ZF_LOGF("Failed to create allocman\n");
	}

	allocman_make_vka(&vka, allocman);

	// get space
	seL4_CPtr cspace_cap;
	cspace_cap = simple_get_cnode(&simple);

	// get vspace
	seL4_CPtr vspace_cap;
	vspace_cap = simple_get_pd(&simple);

	// from seL4test
	// (init_env)
	/* create a vspace (virtual memory management interface). We pass
	 * boot info not because it will use capabilities from it, but so
	 * it knows the address and will add it as a reserved region */
	vspace_t vspace;
	error = sel4utils_bootstrap_vspace_with_bootinfo_leaky(&vspace, &data,
			vspace_cap, &vka, info);
	if (error) {
		ZF_LOGF("Failed to bootstrap vspace\n");
	}

	reservation_t virtual_reservation;
	// fill the allocator with virtual memory
	void *vaddr;
	virtual_reservation = vspace_reserve_range(&vspace,
			ALLOCATOR_VIRTUAL_POOL_SIZE, seL4_AllRights, 1, &vaddr);
	if (virtual_reservation.res == 0) {
		ZF_LOGF("Failed to provide virtual memory for allocator\n");
	}

	bootstrap_configure_virtual_pool(allocman, vaddr,
			ALLOCATOR_VIRTUAL_POOL_SIZE, vspace_cap);

	error = sel4platsupport_new_io_mapper(&vspace, &vka, &io_ops.io_mapper);
	if (error) {
		ZF_LOGF("Failed to get new IO mapper\n");
	}

	error = sel4platsupport_new_malloc_ops(&io_ops.malloc_ops);
	if (error) {
		ZF_LOGF("Failed to get new malloc ops\n");
	}

	platsupport_serial_setup_simple(&vspace, &simple, &vka);

	printf("Done setting up seL4, calling isp_main.\n");
	isp_main();

	return 0;
}
