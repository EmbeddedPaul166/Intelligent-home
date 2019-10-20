	.syntax	unified
	.cpu cortex-m3
    .thumb
	
    .global	__vector_table
    
	.section .vector_table
    
__vector_table:
	.long	_STACK_BEGIN            /* Top of Stack */
	.long	Reset_Handler         /* Reset Handler */
	.long	NMI_Handler           /* NMI Handler */
	.long	HardFault_Handler     /* Hard Fault Handler */
	.long	MemManage_Handler     /* MPU Fault Handler */
	.long	BusFault_Handler      /* Bus Fault Handler */
	.long	UsageFault_Handler    /* Usage Fault Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	SVC_Handler           /* SVCall Handler */
	.long	DebugMon_Handler      /* Debug Monitor Handler */
	.long	0                     /* Reserved */
	.long	PendSV_Handler        /* PendSV Handler */
	.long	SysTick_Handler       /* SysTick Handler */
	.long	External_IRQ_Handler  /* External Interrupt Handler*/

    .weak NMI_Handler
    .thumb_set NMI_Handler, Default_Interrupt_Handler     
    .weak HardFault_Handler
    .thumb_set HardFault_Handler, Default_Interrupt_Handler     
    .weak MemManage_Handler
    .thumb_set MemManage_Handler, Default_Interrupt_Handler    
    .weak BusFault_Handler
    .thumb_set BusFault_Handler, Default_Interrupt_Handler    
    .weak UsageFault_Handler
    .thumb_set UsageFault_Handler, Default_Interrupt_Handler    
    .weak SVC_Handler
    .thumb_set SVC_Handler, Default_Interrupt_Handler    
    .weak DebugMon_Handler
    .thumb_set DebugMon_Handler, Default_Interrupt_Handler    
    .weak PendSV_Handler
    .thumb_set PendSV_Handler, Default_Interrupt_Handler    
    .weak SysTick_Handler
    .thumb_set SysTick_Handler, Default_Interrupt_Handler
    .weak External_IRQ_Handler
    .thumb_set External_IRQ_Handler, Default_Interrupt_Handler

.size __vector_table, .-__vector_table

.section .vector_table

Default_Interrupt_Handler: /*Default handler does nothing, just loops infinitely*/
    infinite_loop:
        B infinite_loop
.size Default_Interrupt_Handler, .-Default_Interrupt_Handler
