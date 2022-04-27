import numpy as np
import cv2 as cv
import time


npTmp = np.random.random((1024, 1024)).astype(np.float32)

npMat1 = np.stack([npTmp,npTmp],axis=2)
npMat2 = npMat1

cuMat1 = cv.cuda_GpuMat()
cuMat2 = cv.cuda_GpuMat()
cuMat1.upload(npMat1)
cuMat2.upload(npMat2)
start_time = time.time()
cv.cuda.gemm(cuMat1, cuMat2,1,None,0,None,1)
print("CUDA --- %s seconds ---" % (time.time() - start_time))
start_time = time.time()

cv.gemm(npMat1,npMat2,1,None,0,None,1)
print("CPU --- %s seconds ---" % (time.time() - start_time))