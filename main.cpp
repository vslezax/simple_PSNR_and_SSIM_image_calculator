#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

class pix{
    public:
    uint8_t b;
    uint8_t g;
    uint8_t r;
    pix(uint8_t a, uint8_t aa, uint8_t aaa){
        b = a;
        g = aa;
        r = aaa;
    }
};

double mean(const std::vector<uint8_t>& img) {
    double sum = 0.0;
    for (auto pixel : img) {
        sum += pixel;
    }
    return sum / img.size();
}

double variance(const std::vector<uint8_t>& img, double mean) {
    double var = 0.0;
    for (auto pixel : img) {
        var += (pixel - mean) * (pixel - mean);
    }
    return var / img.size();
}

double covariance(const std::vector<uint8_t>& img1, const std::vector<uint8_t>& img2, double mean1, double mean2) {
    double cov = 0.0;
    for (size_t i = 0; i < img1.size(); ++i) {
        cov += (img1[i] - mean1) * (img2[i] - mean2);
    }
    return cov / img1.size();
}

double ssim(const std::vector<uint8_t>& img1, const std::vector<uint8_t>& img2) {
    double C1 = 6.5025, C2 = 58.5225;

    double mean1 = mean(img1);
    double mean2 = mean(img2);
    double var1 = variance(img1, mean1);
    double var2 = variance(img2, mean2);
    double covar = covariance(img1, img2, mean1, mean2);

    double ssim = (2 * mean1 * mean2 + C1) * (2 * covar + C2) / ((mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2));
    return ssim;
}

int main() {
    int m = 3840;
    int n = 2160;
    std::string in;
    std::cin >> in;
    std::string out;
    std::cin >> out;

    std::ifstream input(in, std::ios::binary);
    std::ifstream output(out, std::ios::binary);

    // Saving pixels from both images
    int size = m*n;
    std::vector<pix> invec; invec.reserve(size);
    std::vector<pix> outvec; outvec.reserve(size);
    input.seekg(54);
    output.seekg(54);
    for (int i = 0; i < size; i++){
        invec.push_back(pix(input.get(), input.get(), input.get()));
        outvec.push_back(pix(output.get(), output.get(), output.get()));
    }

    // RGB -> Y
    std::vector<uint8_t> a(size);
    std::vector<uint8_t> b(size);
    for (int i = 0; i < size; i++){
        a[i] = invec[i].r * 0.299 + invec[i].g * 0.587 + invec[i].b * 0.114;
        b[i] = outvec[i].r * 0.299 + outvec[i].g * 0.587 + outvec[i].b * 0.114;
    }

    // PSNR calculating
    double psnr = 0;
    for (int i = 0; i < size; i++){
        psnr += pow(a[i] - b[i], 2);
    }
    psnr /= size;
    psnr = 10 * log10(255*255 / psnr);
    std::cout << "PSNR = " << psnr << std::endl;

    input.close();
    output.close();

    // SSIM
    double ssim_value = ssim(a, b);
    std::cout << "SSIM = " << ssim_value << std::endl;
}
