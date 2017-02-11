/*
 * hardware.h
 *
 * Created: 4/02/2017 12:26:36 PM
 *  Author: NormalUser
 */ 
/* Hardware definitions */



#ifndef HARDWARE_H_
#define HARDWARE_H_

#define BIT(x) (1<<x)

/*------OUTPUTS------*/
/* Port A */
#define EN_PWR			BIT(0)
#define CH1_BOOST		BIT(1)
#define CH1_EN			BIT(3)
#define CH2_BOOST		BIT(4)
#define CH2_EN			BIT(6)
#define CH3_BOOST		BIT(7)

/* Port B */
#define CH1_ISET		BIT(4)
#define CH2_ISET		BIT(5)
#define CH3_ISET		BIT(6)
#define CH4_ISET		BIT(7)

/* Port C */
#define WIFI_EN			BIT(0)
#define LED_GRN			BIT(1)
#define LED_RED			BIT(2)
#define CH4_EN			BIT(3)
#define CH4_BOOST		BIT(5)
#define CH3_EN			BIT(6)

/*------INPUTS------*/
/* PORT A */
#define CH1_ILIM		BIT(2)
#define CH2_ILIM		BIT(5)

/* PORT B */

/* PORT C */
#define CH4_ILIM		BIT(4)
#define CH3_ILIM		BIT(7)

/* PORT D */
#define SW_AUTO			BIT(4)
#define SW_OFF			BIT(5)
#define SW_ON			BIT(6)
#define PB_SNOOZE		BIT(7)

/* PORT E */
#define SW_ENC1			BIT(2)
#define SW_ENC2			BIT(3)
#define ENC1B			BIT(4)
#define ENC1A			BIT(5)
#define ENC2B			BIT(6)
#define ENC2A			BIT(7)

#endif /* HARDWARE_H_ */