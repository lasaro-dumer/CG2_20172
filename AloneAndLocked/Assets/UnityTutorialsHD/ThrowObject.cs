using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityStandardAssets.CrossPlatformInput;

public class ThrowObject : MonoBehaviour
{
    public Transform player;
    public Transform playerCam;
    public float throwForce = 10;
    bool hasPlayer = false;
    bool beingCarried = false;
    public AudioClip[] soundToPlay;
    private AudioSource audio;
    public int dmg;
    private bool touched = false;

    void Start()
    {
        audio = GetComponent<AudioSource>();
    }

    void Update()
    {
        if (CrossPlatformInputManager.GetButton("Grab"))
        {
            Ray camRay = new Ray(playerCam.position, playerCam.forward);
            RaycastHit hit;
            if (Physics.Raycast(camRay, out hit, 2.5f))
            {
                var getTarget = hit.transform.gameObject;
                if (getTarget.name == this.name)
                {
                    GetComponent<Rigidbody>().isKinematic = true;
                    transform.parent = playerCam;
                    beingCarried = true;
                }
            }
        }
        if (beingCarried)
        {
            if (touched)
            {
                Drop();
                touched = false;
            }
            else if (CrossPlatformInputManager.GetButton("Fire1"))
            {
                Drop();
                GetComponent<Rigidbody>().AddForce(playerCam.forward * throwForce);
                RandomAudio();
            }
            else if (!CrossPlatformInputManager.GetButton("Grab"))
            {
                Drop();
            }
        }
    }

    private void Drop()
    {
        GetComponent<Rigidbody>().isKinematic = false;
        transform.parent = null;
        beingCarried = false;
    }

    void RandomAudio()
    {
        if (audio == null || audio.isPlaying)
            return;
        audio.clip = soundToPlay[Random.Range(0, soundToPlay.Length)];
        audio.Play();

    }

    void OnTriggerEnter()
    {
        //Debug.Log("triggered");
        if (beingCarried)
        {
            touched = true;
        }
    }
}
