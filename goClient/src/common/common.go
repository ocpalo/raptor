package common

const (
	commonId          = "http://localhost:8080"
	GetServerTime     = commonId + "/api/sunucusaati"
	GetApiLogout      = commonId + "/api/cikis"
	PostSendTelemetry = commonId + "/api/telemetri_gonder"
	PostLockInfo      = commonId + "/api/kilitlenme_bilgisi"
	PostApiLogin      = commonId + "/api/giris"
)

/*
200: İstek başarılı
 204: Gönderilen paketin Formatı Yanlış
 400: İstek hatalı veya geçersiz. Böyle bir durumda hata kodu sayfa içeriği olarak gönderilir.
Hata kodlarının açıklamaları Hata kodları başlığında bulunmaktadır.
 401: Kimliksiz erişim denemesi. Oturum açmanız gerekmektedir.
 403: Yetkisiz erişim denemesi. Yönetici yetkilerine sahip olmayan bir hesap ile
yöneticilere özel bağlantılara giriş yapılmaya çalışmaktadır.
 404: Geçersiz URL.
500: Sunucu içi hata.
*/
const (
	StatusSuccess            = 200
	StatusInvalidRequest     = 400
	StatusLoginRequired      = 401
	StatusUnauthorizedAccess = 403
	StatusInvalidUrl         = 404
	StatusInternalError      = 500
)