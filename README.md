# H264/HEVC Video Encoder for 2D Image

[C++/C# Tutorial: Bring Your Animations to H264/HEVC Video](https://www.codeproject.com/Articles/5161187/Bring-Your-Animations-to-H264-HEVC-Video)

* Hardware acceleration can be turned on after detection.
* Software encoding can be used during testing in VM where hardware acceleration is not available.
* C# code can use the C++/CLI wrapper.

__Note:__ Only even-numbered width framebuffer works due to the NV12 format.

__Note:__ Remember to call CoInitialize() before calling HasH264() and HasHEVC() to detect H264 and HEVC.

## Reference Book

[Developing Microsoft Media Foundation Applications](https://www.amazon.com/gp/product/0735656592/ref=as_li_qf_asin_il_tl?ie=UTF8&tag=qhyti98po-20&creative=9325&linkCode=as2&creativeASIN=0735656592&linkId=3a5b32bf135fdbb0772831efc7e6c96f) by Microsoft Press
