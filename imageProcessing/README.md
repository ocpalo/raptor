
## Requirements
- CUDA
- CUDNN
- OPENCV
## Before installation check versions

> gcc min version 9.3.0

    gcc --version

>kernel min version 5.11.0-27

	uname -r

> Update drivers=>software & updates/additional_drivers select nvidia drive...(proprietary)   [for ubuntu]

	reboot

## CUDA installation

	sudo apt-get install linux-headers-$(uname -r)

	wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/cuda-ubuntu2004.pin

	sudo mv cuda-ubuntu2004.pin /etc/apt/preferences.d/cuda-repository-pin-600

	sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/7fa2af80.pub

	sudo add-apt-repository "deb https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/ /"

	sudo apt-get update

	sudo apt-get -y install cuda

>You should see the cuda version to verify the installation

	ls -lah /usr/local/ | grep cuda


	gedit .bashrc
>Add the below 2 lines to the bottom of the file (bashrc)
- export PATH=/usr/local/cuda/bin${PATH:+:${PATH}}
- export LD_LIBRARY_PATH=/usr/local/cuda/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

>Reboot the system
	
	reboot

>after reboot, you should see the Cuda version with the below command if installation successfully

	nvcc -V


## CUDNN installation

	sudo apt-get install zlib1g

	sudo apt install libcudnn8 libcudnn8-dev

	reboot

	dpkg -l | grep cudnn # kontrol et burdan cudnn versiyonu gormen lazımdı sanırım :=)

**If there is a problem check document for CUDA & CUDNN**
[CUDA](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#pre-installation-actions)
[CUDNN](https://docs.nvidia.com/deeplearning/cudnn/install-guide/index.html#verify)

## OPENCV build

	mkdir opencv_build && cd opencv_build
	wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.5.3.zip
	wget -O opencv.zip https://github.com/opencv/opencv/archive/4.5.3.zip
	unzip opencv.zip
	unzip opencv_contrib.zip
	cd opencv-4.5.3
	mkdir build
	cd build

> Copy all 15 lines below at once
	
	cmake -D CMAKE_BUILD_TYPE=RELEASE \
		-D CMAKE_C_COMPILER=/usr/bin/gcc-10 \
		-D CMAKE_INSTALL_PREFIX=/usr/local \
		-D OPENCV_GENERATE_PKGCONFIG=ON \
		-D BUILD_opencv_python3=ON \
		-D CUDA_ARCH_BIN=7.5 \
		-D WITH_CUDA=ON \
		-D WITH_CUDNN=ON \
		-D OPENCV_DNN_CUDA=ON \
		-D ENABLE_FAST_MATH=1 \
		-D CUDA_FAST_MATH=1 \
		-D OPENCV_ENABLE_NONFREE=ON \
		-D WTIH_CUBLAS=1 \
		-D WITH_V4L=ON \
		-D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib-4.5.3/modules ..
	
	make -j$(nproc)
	sudo make install
	sudo ldconfig




## Test

Run [cuda_vs_cpu_time.py](https://github.com/ocpalo/raptor/tree/main/imageProcessing/build_test)  CPU and CUDA times should appear if the installation is correct.

