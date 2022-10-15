struct __device_t;

typedef struct __fs_t {
	char *name;
	uint8_t (*probe)(struct __device_t *);
	uint8_t (*read)(char *, char *, struct __device_t *, void *);
	uint8_t (*read_dir)(char *, char *, struct __device_t *, void *);
	uint8_t (*touch)(char *fn, struct __device_t *, void *);
	uint8_t (*writefile)(char *fn, char *buf, uint32_t len, struct __device_t *, void *);
	uint8_t (*exist)(char *filename, struct __device_t *, void *);
	uint8_t (*mount)(struct __device_t *, void *);
	uint8_t *priv_data;
} filesystem_t;

typedef enum __device_type {
	DEVICE_UNKNOWN = 0,
	DEVICE_CHAR = 1,
	DEVICE_BLOCK = 2,
} device_type;

typedef struct __device_t {
	char *name;
	uint32_t unique_id;
	device_type dev_type;
	struct __fs_t *fs;
	uint8_t (*read)(uint8_t* buffer, uint32_t offset , uint32_t len, void* dev);
	uint8_t (*write)(uint8_t *buffer, uint32_t offset, uint32_t len, void* dev);
	void *priv;
} device_t;