#include "driver/i2c.h"

#define I2C_MASTER_SCL              22               /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA              21               /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          100000           /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define ADDRESS 0x5C

esp_err_t i2c_master_init(void);