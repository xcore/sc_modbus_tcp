#ifndef HAVE_PLATFORM_H
#define HAVE_PLATFORM_H

#include <xs1.h>

/*
 * Platform description header file.
 * Automatically generated from "C:\Program Files\XMOS\DevelopmentTools\11.2.2\configs\XC-2.xn".
 */

#ifdef __XC__
/* Core array declaration. */
extern core stdcore[4];
#endif

#ifdef __XC__
/* Service prototypes. */
/* none */
#endif

#if !defined(__ASSEMBLER__)
#define PORT_SPI_MISO on stdcore[0]: XS1_PORT_1A
#define PORT_SPI_SS on stdcore[0]: XS1_PORT_1B
#define PORT_SPI_CLK on stdcore[0]: XS1_PORT_1C
#define PORT_SPI_MOSI on stdcore[0]: XS1_PORT_1D
#define PORT_BUTTON_LED_0 on stdcore[0]: XS1_PORT_1E
#define PORT_BUTTON_LED_1 on stdcore[0]: XS1_PORT_1F
#define PORT_UART_RX on stdcore[0]: XS1_PORT_1G
#define PORT_UART_TX on stdcore[0]: XS1_PORT_1H
#define PORT_LED_0_0 on stdcore[0]: XS1_PORT_1I
#define PORT_LED_0_1 on stdcore[0]: XS1_PORT_1J
#define PORT_BUTTON_A on stdcore[0]: XS1_PORT_4C
#define PORT_BUTTON_B on stdcore[0]: XS1_PORT_4D
#define PORT_LED_1_0 on stdcore[1]: XS1_PORT_1I
#define PORT_LED_1_1 on stdcore[1]: XS1_PORT_1J
#define PORT_LED_2_0 on stdcore[2]: XS1_PORT_1I
#define PORT_LED_2_1 on stdcore[2]: XS1_PORT_1J
#define PORT_ETH_TXCLK on stdcore[2]: XS1_PORT_1K
#define PORT_ETH_TXEN on stdcore[2]: XS1_PORT_1L
#define PORT_ETH_RXCLK on stdcore[2]: XS1_PORT_1M
#define PORT_ETH_RXDV on stdcore[2]: XS1_PORT_1N
#define PORT_ETH_RXER on stdcore[2]: XS1_PORT_1O
#define PORT_ETH_MDC on stdcore[2]: XS1_PORT_1P
#define PORT_ETH_RXD on stdcore[2]: XS1_PORT_4E
#define PORT_ETH_TXD on stdcore[2]: XS1_PORT_4F
#define PORT_ETH_RST_N_MDIO on stdcore[2]: XS1_PORT_8D
#define PORT_LED_3_0 on stdcore[3]: XS1_PORT_1I
#define PORT_LED_3_1 on stdcore[3]: XS1_PORT_1J
#else
#define PORT_SPI_MISO XS1_PORT_1A
#define PORT_SPI_SS XS1_PORT_1B
#define PORT_SPI_CLK XS1_PORT_1C
#define PORT_SPI_MOSI XS1_PORT_1D
#define PORT_BUTTON_LED_0 XS1_PORT_1E
#define PORT_BUTTON_LED_1 XS1_PORT_1F
#define PORT_UART_RX XS1_PORT_1G
#define PORT_UART_TX XS1_PORT_1H
#define PORT_LED_0_0 XS1_PORT_1I
#define PORT_LED_0_1 XS1_PORT_1J
#define PORT_BUTTON_A XS1_PORT_4C
#define PORT_BUTTON_B XS1_PORT_4D
#define PORT_LED_1_0 XS1_PORT_1I
#define PORT_LED_1_1 XS1_PORT_1J
#define PORT_LED_2_0 XS1_PORT_1I
#define PORT_LED_2_1 XS1_PORT_1J
#define PORT_ETH_TXCLK XS1_PORT_1K
#define PORT_ETH_TXEN XS1_PORT_1L
#define PORT_ETH_RXCLK XS1_PORT_1M
#define PORT_ETH_RXDV XS1_PORT_1N
#define PORT_ETH_RXER XS1_PORT_1O
#define PORT_ETH_MDC XS1_PORT_1P
#define PORT_ETH_RXD XS1_PORT_4E
#define PORT_ETH_TXD XS1_PORT_4F
#define PORT_ETH_RST_N_MDIO XS1_PORT_8D
#define PORT_LED_3_0 XS1_PORT_1I
#define PORT_LED_3_1 XS1_PORT_1J
#endif


/* Reference frequency definition. */
#define PLATFORM_REFERENCE_HZ 100000000
#define PLATFORM_REFERENCE_KHZ 100000
#define PLATFORM_REFERENCE_MHZ 100

#endif /* HAVE_PLATFORM_H */

