# video_encoder_for_2d_image
H264 Video Encoder for 2D Image

[tutorial](https://www.codeproject.com/Articles/5161187/Bring-Your-Animations-to-H264-HEVC-Video)

Hardware acceleration is available now by adding these 3 lines to set MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS in attributes and pass it to MFCreateSinkWriterFromURL

```
CComPtr<IMFAttributes> attrs;
MFCreateAttributes(&attrs, 1);
attrs->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);

hr = MFCreateSinkWriterFromURL(m_DestFilename.c_str(), nullptr, attrs, &m_pSinkWriter);
```

## Quality parameters in Constructor in v0.4.2

* int numWorkerThreads: 0 leaves to default
* int qualityVsSpeed: [0:100] 0 for speed, 100 for quality
* RateControlMode mode: 3 modes to choose from UnconstrainedVBR, Quality, CBR (VBR is variable bitrate and CBR is constant bitrate)
* int quality: Only valid when mode is Quality. [0:100] 0 for smaller file size and lower quality, 100 for bigger file size and higher quality

## Reference Book

[Developing Microsoft Media Foundation Applications](https://www.amazon.com/gp/product/0735656592/ref=as_li_qf_asin_il_tl?ie=UTF8&tag=qhyti98po-20&creative=9325&linkCode=as2&creativeASIN=0735656592&linkId=3a5b32bf135fdbb0772831efc7e6c96f) by Microsoft Press
