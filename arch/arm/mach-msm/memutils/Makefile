
<<<<<<< HEAD
mach-mem-y		:= memcpy.o copy_from_user.o copy_to_user.o copy_page.o

ifeq ($(CONFIG_ARCH_MSM8974),y)
  mach-mem-y	+= memmove_8974.o
else
  mach-mem-y	+= memmove.o
endif
=======
mach-mem-y		:= memcpy.o copy_from_user.o copy_to_user.o copy_page.o memmove.o

>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
obj-$(CONFIG_HAS_MACH_MEMUTILS) += $(mach-mem-y)
