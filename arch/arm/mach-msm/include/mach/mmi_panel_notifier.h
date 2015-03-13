/*
 * Copyright (C) 2013 Motorola Mobility, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */

#ifndef __MMI_PANEL_NOTIFIER_H__
#define __MMI_PANEL_NOTIFIER_H__

#include <linux/notifier.h>

enum mmi_notifier_enum {
	MMI_PANEL_EVENT_PRE_DISPLAY_OFF,
<<<<<<< HEAD
	MMI_PANEL_EVENT_PRE_DISPLAY_ON,
	MMI_PANEL_EVENT_DISPLAY_OFF,
=======
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
	MMI_PANEL_EVENT_DISPLAY_ON
};

struct mmi_notifier {
#ifdef CONFIG_MMI_PANEL_NOTIFICATIONS
	struct notifier_block mmi_nb;
	struct device *dev;
<<<<<<< HEAD
	int (*pre_display_off)(struct device *dev);
	int (*display_off)(struct device *dev);
	int (*pre_display_on)(struct device *dev);
	int (*display_on)(struct device *dev);
=======
	int (*suspend)(struct device *dev);
	int (*resume)(struct device *dev);
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
#endif
};

#ifdef CONFIG_MMI_PANEL_NOTIFICATIONS
int mmi_panel_register_notifier(struct mmi_notifier *mmi);
int mmi_panel_unregister_notifier(struct mmi_notifier *mmi);
void mmi_panel_notify(unsigned int state, void *data);
#else
#include <linux/errno.h>
static inline int mmi_panel_register_notifier(struct mmi_notifier *mmi)
{
	return -ENOSYS;
}
static inline int mmi_panel_unregister_notifier(struct mmi_notifier *mmi)
{
	return -ENOSYS;
}
static inline void mmi_panel_notify(unsigned int state, void *data) {}
#endif
#endif

