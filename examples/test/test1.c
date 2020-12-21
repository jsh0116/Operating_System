#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <assert.h>

void create_data(const char *filename)
{
	int fd;
	fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	assert(fd != -1);
	write(fd, "hello", 5);
	close(fd);
}

void display_data(const char *filename)
{
	int fd;
	char data;
	fd = open(filename, O_RDONLY);
	assert(fd != -1);
	for(int i=0; i<5; ++i)
		if(read(fd, &data, sizeof(char)) == sizeof(char))
			printf("%c",data);
	printf("\n");
	close(fd);
}

void change_data(const char *filename)
{
	int fd;
	char data;
	fd = open(filename, O_RDWR);
	assert(fd != -1);
	lseek(fd, 4*sizeof(char), SEEK_SET);
	data = '!';
	write(fd, &data, sizeof(char));
	close(fd);
}

void mmap_data(const char *filename)
{
	int fd, page_size;
	char *map;
	fd = open(filename, O_RDWR);
	page_size = getpagesize();
	/*fd로 지정된 파일에서 offset을 시작으로 length만큼 addr로 대응시킴 */
	map = mmap(0,page_size,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	assert(map != MAP_FAILED);

	map[0] &= ~0x20;
	map[1] ^= 0x20;
	map[2] &= 0xDF;
	map[3] ^= ~0xDF;
	msync(map,page_size,MS_ASYNC);
	munmap(map,page_size); //unmap mapping file or device on memory
}

int main(int argc, const char *argv[])
{
        const char *filename;
        assert(argc == 2);
        filename = argv[1];
        create_data(filename);
        display_data(filename);
        change_data(filename);
        display_data(filename);
        mmap_data(filename);
        display_data(filename);
        return EXIT_SUCCESS;
}
