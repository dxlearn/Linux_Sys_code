#define MAXLEN 8
#define MAX_FILES 32
#define MAX_BLOCKSIZE 512

//定义目录项格式
struct dir_entry{
    char filename[MAXLEN];
    uint8_t idx;
}

//定义文件的格式
struct file_blk{
    uint8_t busy;
    uint8_t mode;
    uint8_t idx;

    union 
    {
        uint8_t file_size;
        uint8_t dir_children;
    };
    char data[0];
    
};