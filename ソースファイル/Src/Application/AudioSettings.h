//@file AudioSettings.h

#ifndef AUDIOSETTINGS_H_INCLUDED
#define AUDIOSETTINGS_H_INCLUDED

//�����Đ��v���C���[�ԍ�
namespace AudioPlayer
{
	inline constexpr int bgm = 0;//BGM�̍Đ��Ɏg���v���C���[�ԍ�
	inline constexpr int bossbgm = 1;//BGM�̍Đ��Ɏg���v���C���[�ԍ�
}

//BGM�ݒ�
namespace BGM
{
	inline constexpr char title[]      = "Res/Audio/Opening.mp3";		//�^�C�g����ʂ�BGM
	inline constexpr char load[]       = "Res/Audio/Loading.mp3";		//���[�h��ʂ�BGM
	inline constexpr char stageclear[] = "Res/Audio/StageClear.mp3";	//�Q�[���N���A��ʂ�BGM
	inline constexpr char gameover[]   = "Res/Audio/GameOver.mp3";		//�Q�[���I�[�o�[��ʂ�BGM

	inline constexpr char stage01[]    = "Res/Audio/Stage.mp3";			//�X�e�[�W��BGM
	inline constexpr char boss[]       = "Res/Audio/Boss.mp3";			//�X�e�[�W��BGM


}

//���ʉ��ݒ�
namespace SE
{
	inline constexpr char playerShot[] = "Res/Audio/shoot.wav";			//�e�̔��ˉ�
	inline constexpr char playerExplosion[] = "Res/Audio/BombL.wav";	//���@�̔�����
	inline constexpr char enemyExplosionS[] = "Res/Audio/ExpS.wav";		//�����i���j
	inline constexpr char enemyExplosionM[] = "Res/Audio/ExpM.wav";		//�����i���j
	inline constexpr char enemyExplosionL[] = "Res/Audio/ExpL.wav";		//�����i��j
	inline constexpr char BossExplosion[]   = "Res/Audio/BossExp.wav";	//�{�X�̔�����

	inline constexpr char Click[]		= "Res/Audio/Coin.wav";			//���艹
	inline constexpr char Click2[]      = "Res/Audio/Coin2.wav";		//���艹
	inline constexpr char powerup[]     = "Res/Audio/PowerUp.wav";		//�A�C�e���̎擾��
	inline constexpr char lasttrigger[] = "Res/Audio/LastTrigger.mp3";	//�K�E�Z�̉�


}


#endif //AUDIOSETTINGS_H_INCLUDED