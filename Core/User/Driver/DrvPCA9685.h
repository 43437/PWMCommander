#ifndef __DRV_PCA9885_H_2307010955__
#define __DRV_PCA9885_H_2307010955__

class DrvPCA9685 {
public:
	static DrvPCA9685& Instance();

private:
	DrvPCA9685();
	void Init();
};

#endif //DrvPCA9685
