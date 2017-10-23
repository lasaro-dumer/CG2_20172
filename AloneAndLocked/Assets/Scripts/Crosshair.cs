using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(ObjectInteraction))]
public class Crosshair : MonoBehaviour
{
    private Rect rect;
    private Texture textureNormal;
    private Texture textureActive;
    private ObjectInteraction interactionHandler;
    [Range(0.0f, 10.0f)]
    public float size = 5;
    // Use this for initialization
    void Start()
    {
        float crossHairSize = Screen.width * (size / 100);
        textureNormal = Resources.Load("Textures/crosshair") as Texture;
        textureActive = Resources.Load("Textures/crosshairActive") as Texture;
        float left = Screen.width / 2 - crossHairSize / 2;
        float top = Screen.height / 2 - crossHairSize / 2;
        //left = gameObject.transform.position.x - size / 2;
        //top = gameObject.transform.position.y - size / 2;
        //var p = Camera.main.WorldToScreenPoint(new Vector3(left, top));
        //rect = new Rect(p.x, p.y, size, size);
        rect = new Rect(left, top, crossHairSize, crossHairSize);
        interactionHandler = gameObject.GetComponent<ObjectInteraction>();
    }

    void OnGUI()
    {
        var target = interactionHandler.GetTargettedObject();
        bool canGrab = target != null && interactionHandler.CanGrab(target);
        if (canGrab)
            GUI.DrawTexture(rect, textureActive);
        else
            GUI.DrawTexture(rect, textureNormal);
    }
}
