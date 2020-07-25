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

